#include "verifier.hpp"

#include "../ast.hpp"
#include "../builtin/primitive.hpp"
#include "../frame/frame.hpp"
#include "../frame/starter.hpp"
#include "../frame/thread.hpp"
#include "../loader/errReport.hpp"
#include "../loader/worker/worker.inl"
#include "common/macro.hpp"

namespace nm {

    NM_DEF_ME(verifier)

    namespace {
        static std::vector<str> _getPrimitives() {
            static std::vector<str> inner;
            if(inner.size() <= 0) {
                inner.push_back(new nInt());
                inner.push_back(new nFlt());
                inner.push_back(new nStr());
                inner.push_back(new nByte());
                inner.push_back(new nChar());
                inner.push_back(new nBool());
            };
            return inner;
        }

        nbool checkEvalType(const node& eval) {
            if(nul(eval)) return false;
            for(str e: _getPrimitives())
                if(eval.isSub(*e)) return true;

            return false;
        }

        static ncnt _stepN = 0;
    }

#define _GUARD(msg)                                                                       \
    if(isFlag(GUARD)) do {                                                                \
            NM_I("'%s' %s@%s: " msg, i, ttype<typeTrait<decltype(me)>::Org>::get(), &me); \
            _stepN = 0;                                                                   \
    } while(0)

#define _STEP(msg, ...)                                                                      \
    NM_I("'%s' %s@%s: step#%d --> " msg, i, ttype<typeTrait<decltype(me)>::Org>::get(), &me, \
        ++_stepN, ##__VA_ARGS__)

#define ret_2(code, a1) nothing(), posError(errCode::code, a1)
#define ret_3(code, a1, a2) nothing(), posError(errCode::code, a1, a2)
#define ret_4(code, a1, a2, a3) nothing(), posError(errCode::code, a1, a2, a3)
#define ret_5(code, a1, a2, a3, a4) nothing(), posError(errCode::code, a1, a2, a3, a4)
#define ret(...) NM_OVERLOAD(ret, __VA_ARGS__)

    // verification:
    void me::onLeave(const visitInfo& i, node& me) {
        _GUARD("onLeave()");

        _STEP("no same variable=%d", me.subs().len());
        if(me.isSub<frame>()) return;

        for(auto e = me.subs().begin(); e; ++e) {
            auto matches = me.subAll<baseObj>(e.getKey());

            NM_WHEN(matches.len() > 1).ret(DUP_VAR, *e, e.getKey());
        }
    }

    void me::onLeave(const visitInfo& i, asExpr& me) {
        _GUARD("onVisit()");

        _STEP("_me & _as aren't null");
        NM_WHENNUL(me.getMe()).ret(LHS_IS_NUL, me);
        NM_WHENNUL(me.getAs()).ret(RHS_IS_NUL, me);
        NM_WHEN(me.getAs().isSub<nVoid>()).ret(VOID_NOT_CAST, me);

        _STEP("checks that me can cast to 'as'");
        NM_WHEN(!me.getMe().is(me.getAs())).ret(CAST_NOT_AVAILABLE, me, me.getMe(), me.getAs());

        _STEP("rhs shouldn't be expression");
        NM_WHEN(!me.getAs().isImpli<node>()).ret(CAST_TO_UNKNOWN, me);
    }

    void me::onLeave(const visitInfo& i, assignExpr& me) {
        _GUARD("onVisit()");

        _STEP("set evalType");
        const ntype& ltype = safeGet(me, getLeft(), getEval(), getType());
        NM_WHENNUL(ltype).ret(LHS_IS_NUL, me);
        const ntype& rtype = safeGet(me, getRight(), getEval(), getType());
        NM_WHENNUL(rtype).ret(RHS_IS_NUL, me);
        NM_WHEN(rtype.isSub<retStateExpr>()).ret(CANT_ASSIGN_RET, me);
        NM_WHEN(!rtype.isImpli(ltype)).ret(TYPE_NOT_COMPATIBLE, me, rtype, ltype);

        // verify rvalue and lvalue:
        //  first of all:
        //      in namulang there is no lvalue concept. to involve lvalue concept,
        //      I need to bring new concept or feature like 'const' but I don't want to.
        //      because the direction of this language is a toy lang. DON'T be serious.
        //      but I can't leave something which is going to operate like a lvalue by user.
        //      so I'll emulate the way how lvalue works.
        //
        //  NM_WHEN you assign something:
        //      into left identifier (= lhs), left one should be lvalue. and there are 3 case
        //      of lvalue you can find:
        //
        //      1. getExpr:
        //          assigning identifier registered into some scope is always valid
        //          operation.
        //          e.g. A.B = 5
        //
        //      2. RunExpr:
        //          running some method of object could come in chain of assignment exprs.
        //              e.g. A.B().C = 5
        //          however it shouldn't be the last expression to process.
        //              e.g. A.B() = 5 // invalid
        //
        //      3. ElementExpr:
        //          getting element by index also valid expression of assignment expr.
        //              e.g. A.B[C] = 5
        //          NM_WHEN verifier meets ElementExpr, it doesn't need to deep search any longer to
        //          verify invalidness of lvalue.
        //
        //      it looks complicated to verify this at glance but it's not.
        //      the reason is, structure of expression has been inside out:
        //          e.g. A.B.NAME = 5
        //
        //      the structure of above sample will be,
        //          AssignExpr(getExpr(getExpr(A, B), NAME), nInt(5))
        //      so, only I need to check is, lhs of AssignExpr is kind of getExpr() or not.
        //      I can care about that the last expression is valid.
        _STEP("checks rvalue");
        const node& lhs = me.getLeft();
        NM_WHEN(!lhs.isSub<getExpr>() /*TODO: && !lhs.isSub<ElementExpr>()*/)
            .ret(ASSIGN_TO_RVALUE, me, me.getRight(), lhs);
    }

    void me::onLeave(const visitInfo& i, blockExpr& me) {
        _GUARD("onLeave()");

        _STEP("last stmt should match to ret type");
        const narr& stmts = me.getStmts();
        if(nul(stmts) || stmts.len() <= 0) return; // will be catched to another verification.

        func& parent = safeGet(i.parent, cast<func>());
        if(!nul(parent)) _verifyMgdFuncImplicitReturn(parent);
    }

    void me::_onLeave(const visitInfo& i, const loopExpr& me) {
        _GUARD("onLeave()");

        str eval = me.getEval(); // it's okay forExpr not to have 'eval'.
        if(eval) {
            _STEP("eval Value check: eval[%s] is an array?", eval);
            NM_WHEN(!eval->isSub<retStateExpr>() && !eval->isSub<arr>()).ret(LOOP_NO_RET_ARR, me);
        }

        me.getBlock().outFrame();
        _recentLoops.pop_back();
    }

    void me::onLeave(const visitInfo& i, defVarExpr& me) {
        _GUARD("onLeave()");

        _STEP("to define a void type property isn't allowed.");
        str eval = safeGet(me.getRight(), getEval());
        NM_WHEN(!eval).ret(RHS_IS_NUL, me);
        NM_WHEN(eval->isSub<nVoid>()).ret(VOID_CANT_DEFINED, me);

        obj& cast = eval->cast<obj>();
        if(!nul(cast)) {
            const baseObj& org = cast.getOrigin();
            NM_WHEN(!org.isPreEvaluated()).ret(TYPE_IS_NOT_PRE_EVALUATED, me, me.getName());
        }

        _STEP("does rhs[%s] have 'ret' in its blockStmt?", eval);
        NM_WHEN(eval->isSub<retStateExpr>()).ret(CANT_ASSIGN_RET, me);

        _STEP("check whether make a void container.");
        const narr& beans = eval->getType().getBeans();
        for(const node& bean: beans)
            NM_WHEN(bean.isSub<nVoid>()).ret(NO_VOID_CONTAINER, me);

        std::string name = me.getName();
        NM_WHEN(name == "").ret(HAS_NO_NAME, me);

        _STEP("is %s definable?", name);
        const ntype& t = eval->getType();
        if(nul(t)) posError(errCode::CANT_DEF_VAR, me, name.c_str(), "null");
        NM_WHEN(eval->isSub<nVoid>()).ret(VOID_CANT_DEFINED, me);

        node& to = me.getTo();
        _STEP("is 'to'[%s] valid", to);
        // only if to is 'frame', I need to make property NM_WHEN verify:
        //  local variables are required to verify further statements. but it's okay. it'll been
        //  removed after blockExpr::outFrame().
        //
        //  however, if you make a variable on defBlock, that matters and affects rest of real
        //  execution of codes. because they still leaves after verification because they aren't on
        //  local scope.
        //  but don't worry. these kind of properties have to be evaluated and instantiated on
        //  the pre-evaluation step.
        if(!nul(to)) return;

        frame& fr = thread::get()._getNowFrame();
        _STEP("duplication of variable with name[%s]", name);
        NM_WHEN(fr.mySubs()->has(name)).ret(ALREADY_DEFINED_VAR, me, name, t);

        _STEP("ok. defining local temporary var... %s %s", name, t);
        fr.addLocal(name, *new mockNode(*eval));
    }

    void me::onLeave(const visitInfo& i, defAssignExpr& me) {
        _GUARD("onVisit()");

        str eval = safeGet(me, getRight(), getEval());
        NM_WHEN(!eval).ret(RHS_IS_NUL, me);
        NM_WHEN(!eval->isComplete()).ret(ACCESS_TO_INCOMPLETE, me);

        onLeave(i, (defAssignExpr::super&) me);
    }

    void me::onLeave(const visitInfo& i, defSeqExpr& me) {
        _GUARD("onVisit()");

        _STEP("check lhs & rhs");
        auto& start = me.getStart();
        NM_WHENNUL(start).ret(LHS_IS_NUL, me);
        auto& end = me.getEnd();
        NM_WHENNUL(end).ret(RHS_IS_NUL, me);

        _STEP("lhs & rhs is sort of Int?");
        NM_WHEN(!start.isImpli<nInt>()).ret(SEQ_SHOULD_INT_COMPATIBLE, me);
        NM_WHEN(!end.isImpli<nInt>()).ret(SEQ_SHOULD_INT_COMPATIBLE, me);
    }

    void me::onLeave(const visitInfo& i, defArrayExpr& me) {
        _GUARD("onVisit()");

        _STEP("check all elements");
        const node& type = me.getArrayType();
        NM_WHENNUL(type).ret(ELEM_TYPE_DEDUCED_NUL, me);
        NM_WHEN(type.isSuper<obj>()).ret(ELEM_TYPE_DEDUCED_WRONG, me, type);
        NM_WHEN(type.isSub<nVoid>()).ret(ELEM_TYPE_NOT_VOID, me);

        _STEP("check arr has exactly 1 type parameter.");
        const auto& beans = safeGet(me.getOrigin(), getType(), getBeans());
        NM_WHENNUL(beans).ret(ELEM_TYPE_IS_NUL, me);
        NM_WHEN(beans.len() != 1).ret(ARR_DOESNT_HAVE_TYPE_PARAM, me);
    }

    void me::onLeave(const visitInfo& i, FBOExpr& me) {
        _GUARD("onVisit()");

        _STEP("finding eval of l(r)hs.");
        str lEval = safeGet(me, getLeft(), getEval());
        str rEval = safeGet(me, getRight(), getEval());
        NM_WHEN(!lEval).ret(LHS_IS_NUL, me);
        NM_WHEN(!rEval).ret(RHS_IS_NUL, me);

        NM_WHEN(!checkEvalType(*lEval)).ret(LHS_IS_NOT_ARITH, me, lEval);
        NM_WHEN(!checkEvalType(*rEval)).ret(RHS_IS_NOT_ARITH, me, rEval);

        NM_WHENNUL(lEval->deduce(*rEval)).ret(IMPLICIT_CAST_NOT_AVAILABLE, me, lEval, rEval);

        auto r = me.getRule();
        if((lEval->isSub<nStr>() || rEval->isSub<nStr>())) {
            switch(r) {
                case FBOExpr::AND:
                case FBOExpr::OR:
                case FBOExpr::SUB:
                case FBOExpr::DIV:
                case FBOExpr::MOD:
                case FBOExpr::BITWISE_AND:
                case FBOExpr::BITWISE_XOR:
                case FBOExpr::BITWISE_OR:
                case FBOExpr::LSHIFT:
                case FBOExpr::RSHIFT: return posError(errCode::STRING_IS_NOT_PROPER_TO_OP, me);

                default:;
            }
        }
    }

    void me::onLeave(const visitInfo& i, FUOExpr& me) {
        _GUARD("onLeave()");

        _STEP("string isn't proper to any FUO operator");
        str eval = me.getEval();
        NM_WHEN(eval && eval->isImpli<nStr>()).ret(STRING_IS_NOT_PROPER_TO_OP, me);
    }

    void me::onLeave(const visitInfo& i, getExpr& me) {
        _GUARD("onLeave()");

        // TODO: I have to check that the evalType has what matched to given _params.
        // Until then, I rather use as() func and it makes slow emmersively.
        _STEP("isRunnable: %s.%s", me, me.getName());
        NM_WHEN(!me.getEval()).ret(WHAT_IS_THIS_IDENTIFIER, me, me.getName());
        auto matches = me._get(true);
        if(matches.isEmpty()) {
            const node& from = me.getMe();
            return posError(errCode::CANT_ACCESS, me, me._name.c_str(),
                from.getType().getName().c_str());
        }
        node& got = matches.get();
        // TODO: leave logs for all ambigious candidates as err.
        NM_WHENNUL(got).ret(AMBIGIOUS_ACCESS, me, i);

        _STEP("isRunnable: got=%s, me=%s", got, me.getType());

        str asedMe = me.getMe().getEval();
        _STEP("accesses to incomplete 'me[%s]' object?", asedMe);
        NM_WHEN(asedMe && !asedMe->isComplete()).ret(ACCESS_TO_INCOMPLETE, me);
    }

    void me::onLeave(const visitInfo& i, retExpr& me) {
        _GUARD("onVisit()");

        _STEP("should be at last stmt");
        NM_WHEN(i.index != i.len - 1).ret(RET_AT_MIDDLE_OF_BLOCK, me);

        _STEP("checks evalType of func is matched to me");
        const baseFunc& f = thread::get().getNowFrame().getFunc();
        NM_WHENNUL(f).ret(NO_FUNC_INFO, me);

        str myRet = me.getRet().getEval();
        NM_WHEN(!myRet).ret(EXPR_EVAL_NUL, me);

        str funRet = f.getRet()->as<node>();
        _STEP("checks return[%s] == func[%s]", myRet, funRet);

        NM_WHEN(!myRet->isSub<err>() && !myRet->isImpli(*funRet))
            .ret(RET_TYPE_NOT_MATCH, me, myRet, funRet);
    }

    void me::onLeave(const visitInfo& i, runExpr& me) {
        _GUARD("onVisit()");

        _STEP("is it possible to run?");
        NM_WHENNUL(me.getMe()).ret(DONT_KNOW_ME, me);

        str ased = me.getMe().getEval();
        if(!ased) return;

        args& a = me.getArgs();
        a.setMe(*ased);

        node& anySub = me.getSubj();
        NM_WHENNUL(anySub).ret(FUNC_NOT_EXIST, me);
        _STEP("anySub[%s]", anySub);

        str derivedSub = anySub.getEval();
        NM_WHEN(!derivedSub).ret(CANT_ACCESS, me, ased, "sub-node");

        _STEP("derivedSub[%s]", derivedSub);
        if(!derivedSub->canRun(me.getArgs())) {
            const baseFunc& derivedCast = derivedSub->cast<baseFunc>();
            std::string params = nul(derivedCast) ? "ctor" : _asStr(derivedCast.getParams());
            return posError(errCode::OBJ_WRONG_ARGS, me, i.name.c_str(),
                me.getArgs().asStr().c_str(), params.c_str());
        }

        a.setMe(nulOf<baseObj>());
    }

    void me::onTraverse(runExpr& me, node& subject) {
        str ased = safeGet(me.getMe(), getEval());
        if(!ased) return;

        getExpr& cast = subject.cast<getExpr>();
        if(!nul(cast)) cast.setMe(*ased);
    }

    std::string me::_asStr(const params& ps) {
        std::string ret;
        nbool first = true;
        for(const param& p: ps) {
            ret += (first ? "" : ",") + p.getOrigin().getType().getName();
            first = false;
        }

        return ret;
    }

    nbool me::onVisit(const visitInfo& i, func& me) {
        _GUARD("onVisit()");

        onLeave(i, (func::super&) me);

        ctor& c = me.cast<ctor>();
        NM_WHEN(!nul(c) && !c.getRet()).ret(CTOR_NOT_IN_DEF_OBJ, me), true;

        obj& meObj =
            thread::get()
                ._getNowFrame()
                .getMe()
                .cast<obj>(); // TODO: same to 'thread::get().getNowFrame().getMe().cast<obj>();'
        NM_WHENNUL(meObj).ret(FUNC_REDIRECTED_OBJ, me), true;

        _STEP("check func duplication");
        const nbicontainer& top = meObj.getShares().getContainer();
        ncnt len = me.getParams().len();
        const node& errFound = top.get([&](const std::string& key, const node& val) {
            if(key != i.name) return false;
            if(&val == &me) return false;

            const baseFunc& cast = val.cast<baseFunc>();
            if(nul(cast))
                // this has same name on shares, but it's not func! it's not valid.
                // this could be an origin obj.
                return true;

            const params& castPs = cast.getParams();
            if(castPs.len() != len) return false;

            for(int n = 0; n < castPs.len(); n++) {
                str lhs = castPs[n].getOrigin().getEval();
                str rhs = me.getParams()[n].getOrigin().getEval();
                if(lhs->getType() != rhs->getType()) return false;
            }

            return true;
        });
        if(!nul(errFound)) posError(errCode::ALREADY_DEFINED_FUNC, me, i.name.c_str());

        //  obj or property shouldn't have same name to any func.
        _STEP("check func has same name to field");
        if(!nul(meObj.getOwns().get(i.name)))
            posError(errCode::ALREADY_DEFINED_IDENTIFIER, me, i.name.c_str());

        _STEP("main func return type should be int or void");
        if(i.name == starter::MAIN) {
            str ret = me.getRet();
            if(!ret->isSub<nInt>() && !ret->isSub<nVoid>())
                posError(errCode::MAIN_FUNC_RET_TYPE, me);
        }

        _STEP("retType exists and stmts exist one at least");
        str retType = me.getRet();
        if(!retType) return posError(errCode::NO_RET_TYPE, me), true;
        NM_WHEN(!retType->isSub(ttype<node>::get())).ret(WRONG_RET_TYPE, me, retType), true;

        blockExpr& blk = (blockExpr&) me.getBlock();
        if(nul(blk) || blk.getStmts().len() <= 0) {
            if(i.name == starter::MAIN) posError(errCode::MAIN_SHOULD_HAVE_STMTS, blk);
            /*TODO: uncomment after implement abstract:
            if(!func.isAbstract() && !retType->isSub<nVoid>())
                error(...)
            */
            return true;
        }

        _STEP("'break' or 'next' can't be used to last stmt");
        const node& lastStmt = *blk.getStmts().last();
        NM_WHEN(lastStmt.isSub<retStateExpr>() && !lastStmt.isSub<retExpr>())
            .ret(FUNC_SHOULD_RETURN_SOMETHING, lastStmt),
            true;

        _STEP("func[%s]: %s iterateBlock[%d]", i, me, me._blk->subs().len());

        // sequence of adding frame matters:
        //  object scope was added at 'onVisit(visitInfo, baseObj&)
        //  parameters of func is second:
        scope* s = new scope();
        for(const auto& p: me.getParams()) {
            if(p.getOrigin().isSub<nVoid>()) {
                posError(errCode::PARAM_NOT_VOID, me, p.getName().c_str());
                continue;
            }
            s->add(p.getName(), *(node*) p.getOrigin().getEval()->clone());
        }

        //  function's subs are third:
        me.inFrame(*s);
        return true;
    }

    void me::_verifyMgdFuncImplicitReturn(func& me) {
        str ret = me.getRet()->as<node>();
        const type& retType = ret->getType();
        const node& lastStmt = *me.getBlock().getStmts().last();

        if(retType == ttype<nVoid>::get())
            return NM_I("func: implicit return won't verify NM_WHEN retType is void."), void();

        str eval = me.getBlock().getEval();
        NM_WHEN(!eval).ret(NO_RET_TYPE, lastStmt);

        const ntype& lastType = eval->getType(); // to get type of expr, always uses evalType.
        NM_WHENNUL(lastType).ret(NO_RET_TYPE, lastStmt);

        NM_I("func: last stmt[%s] should matches to return type[%s]", eval, retType);

        if(eval->isSub<retStateExpr>())
            // @see retExpr::getEval() for more info.
            return NM_I("func: skip verification NM_WHEN lastStmt is retStateExpr."), void();

        NM_WHEN(!lastType.isSub<err>() && !lastType.isImpli(retType))
            .ret(RET_TYPE_NOT_MATCH, lastStmt, lastType, retType);
    }

    void me::_prepare() {
        super::_prepare();
        _recentLoops.clear();
    }

    void me::onLeave(const visitInfo& i, func& me) {
        _GUARD("onLeave()");

        me.outFrame(scope());
    }

    nbool me::onVisit(const visitInfo& i, baseObj& me) {
        _GUARD("onVisit()");

        me.inFrame();

        frame& fr = thread::get()._getNowFrame();
        _STEP("%s push me[%s] len=%d", fr.getMe(), (void*) &fr.getMe(), me.subs().len());

        _STEP("iterate all subs and checks void type variable");
        for(const node& elem: me.subs())
            if(elem.isSub<nVoid>()) posError(errCode::VOID_CANT_DEFINED, elem);

        onLeave(i, (baseObj::super&) me);
        return true;
    }

    void me::onLeave(const visitInfo& i, baseObj& me) {
        _GUARD("onLeave()");
        me.outFrame();
    }

    nbool me::onVisit(const visitInfo& i, genericOrigin& me) {
        _GUARD("onVisit()");

        _STEP("cache check");
        for(auto e: me._cache)
            if(nul(e.second)) posError(errCode::MAKE_GENERIC_FAIL, me, e.first.c_str());
        return true;
    }

    void me::onLeave(const visitInfo& i, genericOrigin& me) {
        _GUARD("onLeave()");

        // DO NOTHING, BUT LEAVE THIS FUNC:
        //  if I don't have this func, getGenericExpr::super (=baseObj)'s one will be called.
        //  and me pointer will be erased too inside the func.
    }

    nbool me::onVisit(const visitInfo& i, forExpr& me) {
        _GUARD("onVisit()");

        str container = me._container;
        str conAsed = container->getEval();
        NM_WHEN(!conAsed).ret(CONTAINER_IS_NUL, me), true;
        str elemType = conAsed->run("getElemType");
        NM_WHEN(!elemType).ret(ELEM_TYPE_IS_NUL, me), true;

        const std::string& name = me.getLocalName();
        _STEP("define iterator '%s %s'", elemType, name);

        me.getBlock().inFrame();
        thread::get()._getNowFrame().addLocal(name, *((node*) elemType->clone()));
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(const visitInfo& i, forExpr& me) {
        _GUARD("onLeave()");

        _onLeave(i, me);
    }

    nbool me::onVisit(const visitInfo& i, whileExpr& me) {
        _GUARD("onVisit()");

        me.getBlock().inFrame();
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(const visitInfo& i, whileExpr& me) {
        _GUARD("onLeave()");

        _onLeave(i, me);
    }

    void me::onLeave(const visitInfo& i, breakExpr& me) {
        _GUARD("onVisit()");

        _STEP("declared outside of loop?");
        NM_WHEN(_recentLoops.size() <= 0).ret(BREAK_OUTSIDE_OF_LOOP, me);
    }

    void me::onLeave(const visitInfo& i, nextExpr& me) {
        _GUARD("onVisit()");

        _STEP("declared outside of loop?");
        NM_WHEN(_recentLoops.size() <= 0).ret(NEXT_OUTSIDE_OF_LOOP, me);
    }

    nbool me::onVisit(const visitInfo& i, ifExpr& me) {
        _GUARD("onVisit()");
        me.getThen().inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, ifExpr& me) {
        _GUARD("onLeave()");
        blockExpr()
            .outFrame(); // it doesn't matter getting blockExpr from 'me'.
                         // because conceptually, blockExpr::outFrame() is just like static func.
    }

    void me::onTraverse(ifExpr& me, blockExpr& blk) {
        if(nul(blk)) return;

        me.getThen().outFrame();
        blk.inFrame();
    }
} // namespace nm
