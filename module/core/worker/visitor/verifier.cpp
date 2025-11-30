#include "core/worker/visitor/verifier.hpp"
#include "core/ast.hpp"
#include "core/builtin/primitive.hpp"
#include "core/frame/frame.hpp"
#include "core/frame/starter.hpp"
#include "core/frame/thread.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/worker/worker.inl"

namespace by {

    BY_DEF_ME(verifier)

    namespace {
        static std::vector<str> _getPrimitives() {
            static std::vector<str> inner;
            if(inner.size() <= 0) {
                inner.push_back(new nInt());
                inner.push_back(new nFlt());
                inner.push_back(new nStr());
                inner.push_back(new nByte());
                inner.push_back(new nBool());
            };
            return inner;
        }

        nbool checkEvalType(const node* evalable) {
            const node& eval = evalable OR.ret(false);
            for(str e: _getPrimitives())
                if(eval.isSub(*e)) return true;

            return false;
        }

        static ncnt _stepN = 0;
    }

#define _GUARD(msg)                                         \
    if(isFlag(GUARD)) do {                                  \
            BY_I("'%s' %s@%s: " msg, i, me.getType(), &me); \
            _stepN = 0;                                     \
    } while(0)

#define _STEP(msg, ...) \
    BY_I("'%s' %s@%s: step#%d --> " msg, i, ttype<typeTrait<decltype(me)>::Org>::get(), &me, ++_stepN, ##__VA_ARGS__)

    // verification:
    void me::onLeave(const visitInfo& i, node& me, nbool) {
        _GUARD("onLeave(node&)");

        _STEP("no same variable=%d", me.subs().len());
        WHEN(me.isSub<frame>()) .ret();

        for(auto e = me.subs().begin(); e; ++e) {
            auto matches = me.subAll<baseObj>(e.getKey());

            WHEN(matches.len() > 1) .myExErr(*e, DUP_VAR, e.getKey()).ret();
        }
    }

    void me::onLeave(const visitInfo& i, asExpr& me, nbool) {
        _GUARD("onLeave(asExpr&)");

        _STEP("_me & _as aren't null");
        WHEN(me.getAs().isSub<nVoid>()) .myExErr(me, VOID_NOT_CAST).ret();

        _STEP("checks that me can cast to 'as'");
        WHEN(!me.getMe().is(me.getAs())) .myExErr(me, CAST_NOT_AVAILABLE, me.getMe(), me.getAs()).ret();

        _STEP("rhs shouldn't be expression");
        WHEN(!me.getAs().isImpli<node>()) .myExErr(me, CAST_TO_UNKNOWN).ret();
    }

    void me::onLeave(const visitInfo& i, isExpr& me, nbool) {
        _GUARD("onLeave(isExpr&)");

        _STEP("checks that me can cast to 'as'");
        WHEN(!me.getMe().is(me.getTo())) .myExErr(me, CAST_NOT_AVAILABLE, me.getMe(), me.getTo()).ret();

        _STEP("rhs shouldn't be expression");
        WHEN(!me.getTo().isImpli<node>()) .myExErr(me, CAST_TO_UNKNOWN).ret();
    }

    void me::onLeave(const visitInfo& i, assignExpr& me, nbool) {
        _GUARD("onLeave(assignExpr&)");

        _STEP("set evalType");

        str left = me.getLeft().infer() OR.myExErr(me, LHS_IS_NUL).ret();
        const ntype& ltype = left->getType();
        str right = me.getRight().infer() OR.myExErr(me, RHS_IS_NUL).ret();
        const ntype& rtype = right->getType();
        WHEN(rtype.isSub<retStateExpr>()) .myExErr(me, CANT_ASSIGN_RET).ret();
        WHEN(!rtype.isImpli(ltype)) .myExErr(me, TYPE_NOT_COMPATIBLE, rtype, ltype).ret();

        // verify rvalue and lvalue:
        //  first of all:
        //      in byeolang there is no lvalue concept. to involve lvalue concept,
        //      I need to bring new concept or feature like 'const' but I don't want to.
        //      because the direction of this language is a toy lang. DON'T be serious.
        //      but I can't leave something which is going to operate like a lvalue by user.
        //      so I'll emulate the way how lvalue works.
        //
        //  WHEN you assign something:
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
        //          WHEN verifier meets ElementExpr, it doesn't need to deep search any longer to
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
        const getExpr& lhs = me.getLeft().cast<getExpr>() OR.myExErr(me, ASSIGN_TO_RVALUE, me.getRight(), lhs).ret();

        _STEP("checks that you can't assign to a func");
        str lhsEval = lhs.infer();
        WHEN(baseFunc::isFuncButNotClosure(*lhsEval)) .myExErr(me, ASSIGN_TO_FUNC).ret();

        _STEP("checks that try to assign to a const variable");
        WHEN(util::checkTypeAttr(lhs.getName()) == ATTR_CONST) .myExErr(me, ASSIGN_TO_CONST, lhs.getName()).ret();
    }

    nbool me::onVisit(const visitInfo& i, blockExpr& me, nbool) {
        _GUARD("onVisit(blockExpr&)");

        me.inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, blockExpr& me, nbool) {
        _GUARD("onLeave(blockExpr&)");

        const narr& stmts = me.getStmts();
        WHEN(stmts.len() <= 0) .ret(); // will be catched to another verification.

        me.setEval(me.infer().get());
        me.outFrame();
    }

    void me::_onLeave(const visitInfo& i, const loopExpr& me) {
        _recentLoops.pop_back();

        str eval = me.infer(); // it's okay forExpr not to have 'eval'.
        if(eval) {
            _STEP("eval Value check: eval[%s] is an array?", eval);
            WHEN(!eval->isSub<retStateExpr>() && !eval->isSub<arr>()) .myExErr(me, LOOP_NO_RET_ARR).ret();
        }
    }

    void me::onLeave(const visitInfo& i, defVarExpr& me, nbool) {
        _GUARD("onLeave(defVarExpr&)");

        _STEP("modifier not allowed for local variables in a func.");
        const auto& mod = me.getNewModifier();
        frame& fr = thread::get()._getNowFrame() OR.myExErr(me, THERE_IS_NO_FRAMES_IN_THREAD).ret();
        const baseFunc* fun = fr.getFunc();
        nbool isInLocal = !fun || fun->getSrc().getName() != baseObj::EXPAND_NAME;
        WHEN(isInLocal && !mod.isPublic()) .myExErr(me, PROTECTED_NOT_ALLOWED_FOR_LOCAL, me.getName()).ret();
        WHEN(mod.isExplicitOverride()) .myExErr(me, OVERRIDE_NOT_ALLOWED_FOR_LOCAL, me.getName()).ret();

        _STEP("to define a void type property isn't allowed.");
        str eval = me.getRight() TO(infer()) OR.myExErr(me, RHS_IS_NUL).ret();
        WHEN(eval->isSub<nVoid>()) .myExErr(me, VOID_CANT_DEFINED).ret();

        obj* cast = eval->cast<obj>();
        if(cast) {
            const baseObj& org = cast->getOrigin();
            WHEN(org.getState() < PARSED) .myExErr(me, TYPE_IS_NOT_PRE_EVALUATED, me.getName()).ret();
        }

        _STEP("does rhs[%s] have 'ret' in its blockStmt?", eval);
        WHEN(eval->isSub<retStateExpr>()) .myExErr(me, CANT_ASSIGN_RET).ret();

        _STEP("check whether make a void container.");
        const auto& ps = eval->getType().getParams();
        for(const auto& p: ps)
            WHEN(p.getOrigin().isSub<nVoid>()) .myExErr(me, NO_VOID_CONTAINER).ret();

        std::string name = me.getName();
        WHEN(name == "") .myExErr(me, HAS_NO_NAME).ret();

        _STEP("is %s definable?", name);
        const ntype& t = eval->getType();
        WHEN(eval->isSub<nVoid>()) .myExErr(me, VOID_CANT_DEFINED).ret();

        node* to = me.getTo();
        _STEP("is 'to'[%s] valid", to);
        // only if to is 'frame', I need to make property WHEN verify:
        //  local variables are required to verify further statements. but it's okay. it'll been
        //  removed after blockExpr::outFrame().
        //
        //  however, if you make a variable on defBlock, that matters and affects rest of real
        //  execution of codes. because they still leaves after verification because they aren't on
        //  local scope.
        //  but don't worry. these kind of properties have to be evaluated and instantiated on
        //  the pre-evaluation step.
        WHEN(to) .ret();

        _STEP("duplication of variable with name[%s]", name);
        WHEN(fr.mySubs()->in(name)) .myExErr(me, ALREADY_DEFINED_VAR, name, t).ret();

        _STEP("ok. defining local temporary var... %s %s", name, t);
        fr.addLocal(name, *new mockNode(*eval));
    }

    void me::onLeave(const visitInfo& i, defPropExpr& me, nbool) {
        _GUARD("onLeave(defPropExpr&)");

        _STEP("whether the 'type' object has a ctor without any paramters?");
        str eval = me TO(getRight()) TO(infer()) OR.myExErr(me, RHS_IS_NUL).ret();
        WHEN(eval->isSub<baseObj>() && !eval->sub(ctor::CTOR_NAME, args())) .myExErr(me, DONT_HAVE_CTOR, eval).ret();
        func* fun = eval->cast<func>();
        WHEN(fun && fun->isAbstract()) .myExErr(me, YOU_CANT_DEFINE_PROPERTY_WITH_ABSTRACT_FUNC).ret();

        onLeave(i, (defPropExpr::super&) me, false);
    }

    void me::onLeave(const visitInfo& i, defAssignExpr& me, nbool) {
        _GUARD("onLeave(defAssignExpr&)");

        _STEP("check rhs");
        str eval = me TO(getRight()) TO(infer()) OR.myExErr(me, RHS_IS_NUL).ret();
        WHEN(!eval->isComplete()) .myExErr(me, ACCESS_TO_INCOMPLETE).ret();

        const node* explicitType = me TO(getExplicitType());
        if(explicitType) {
            _STEP("check explicit type whether it's valid");
            str type = me TO(infer());
            WHEN_NUL(type).myExErr(me, EXPLICIT_TYPE_SHOULDNT_BE_NULL).ret();
            WHEN(type->isSub<nVoid>()) .myExErr(me, NO_VOID_VARIABLE).ret();
        }

        onLeave(i, (defAssignExpr::super&) me, false);
    }

    void me::onLeave(const visitInfo& i, defSeqExpr& me, nbool) {
        _GUARD("onLeave(defSeqExpr&)");

        _STEP("check lhs & rhs");
        auto& start = me.getStart();
        auto& end = me.getEnd();

        _STEP("lhs & rhs is sort of Int?");
        WHEN(!start.isImpli<nInt>()) .myExErr(me, SEQ_SHOULD_INT_COMPATIBLE).ret();
        WHEN(!end.isImpli<nInt>()) .myExErr(me, SEQ_SHOULD_INT_COMPATIBLE).ret();
    }

    void me::onLeave(const visitInfo& i, defArrayExpr& me, nbool) {
        _GUARD("onLeave(defArrayExpr&)");

        _STEP("check all elements");
        const node& type = me.getArrayType() OR.myExErr(me, ELEM_TYPE_DEDUCED_NUL).ret();
        WHEN(type.isSuper<obj>()) .myExErr(me, ELEM_TYPE_DEDUCED_WRONG, type).ret();
        WHEN(type.isSub<nVoid>()) .myExErr(me, ELEM_TYPE_NOT_VOID).ret();

        _STEP("check arr has exactly 1 type parameter.");
        const auto& ps = me.getOrigin().getType().getParams();
        WHEN(ps.len() != 1) .myExErr(me, ARR_DOESNT_HAVE_TYPE_PARAM).ret();
    }

    void me::onLeave(const visitInfo& i, defNestedFuncExpr& me, nbool) {
        _GUARD("onLeave(defNestedFuncExpr&)");

        _STEP("does it have `eval`?");
        frame& fr = thread::get()._getNowFrame() OR.myExErr(me, THERE_IS_NO_FRAMES_IN_THREAD).ret();
        str eval = me.infer();
        WHEN_NUL(eval).myExErr(me, EXPR_EVAL_NUL).ret();

        _STEP("does it have a name?");
        const std::string& name = me.getOrigin().getSrc().getName();
        WHEN_NUL(name).myExErr(me, HAS_NO_NAME).ret();

        _STEP("define nested `%s` func in local", name);
        fr.addLocal(name, *eval);
    }

    void me::onLeave(const visitInfo& i, FBOExpr& me, nbool) {
        _GUARD("onLeave(FBOEXpr&)");

        _STEP("finding eval of l(r)hs.");
        str lEval = me.getLeft().infer() OR.myExErr(me, LHS_IS_NUL).ret();
        str rEval = me.getRight().infer() OR.myExErr(me, RHS_IS_NUL).ret();

        WHEN(!checkEvalType(lEval.get())) .myExErr(me, LHS_IS_NOT_ARITH, lEval).ret();
        WHEN(!checkEvalType(rEval.get())) .myExErr(me, RHS_IS_NOT_ARITH, rEval).ret();

        WHEN_NUL(lEval->promote(*rEval)).myExErr(me, IMPLICIT_CAST_NOT_AVAILABLE, lEval, rEval).ret();

        auto r = me.getSymbol();
        if((lEval->isSub<nStr>() || rEval->isSub<nStr>())) {
            switch(r) {
                case FBOExpr::SYMBOL_AND:
                case FBOExpr::SYMBOL_OR:
                case FBOExpr::SYMBOL_SUB:
                case FBOExpr::SYMBOL_DIV:
                case FBOExpr::SYMBOL_MOD:
                case FBOExpr::SYMBOL_BITWISE_AND:
                case FBOExpr::SYMBOL_BITWISE_XOR:
                case FBOExpr::SYMBOL_BITWISE_OR:
                case FBOExpr::SYMBOL_LSHIFT:
                case FBOExpr::SYMBOL_RSHIFT: return BY_WHEN.myExErr(me, STRING_IS_NOT_PROPER_TO_OP).ret();

                default:;
            }
        }
    }

    void me::onLeave(const visitInfo& i, FUOExpr& me, nbool) {
        _GUARD("onLeave(FUOExpr&)");

        _STEP("string isn't proper to any FUO operator");
        str eval = me.infer();
        WHEN(eval && eval->isImpli<nStr>()) .myExErr(me, STRING_IS_NOT_PROPER_TO_OP).ret();
    }

    void me::onLeave(const visitInfo& i, getExpr& me, nbool) {
        _GUARD("onLeave(getExpr&)");

        // TODO: I have to check that the evalType has what matched to given _params.
        // Until then, I rather use as() func and it makes slow emmersively.
        _STEP("isRunnable: %s.%s", me, me.getName());
        WHEN(!me.infer()).myExErr(me, WHAT_IS_THIS_IDENTIFIER, me.getName()).ret();
        str match = me._get(true) OR_DO {
            const node* from = me.getMe();
            return BY_WHEN.myExErr(me, CANT_ACCESS, me._name.c_str(), from TO(getType().getName().c_str())).ret();
        }

        // TODO: leave logs for all ambigious candidates as err.
        WHEN_NUL(match).myExErr(me, AMBIGIOUS_ACCESS, i).ret();

        _STEP("isRunnable: got=%s, me=%s", match, me.getType());

        str asedMe = me.getMe() TO(infer());
        _STEP("accesses to incomplete 'me[%s]' object?", asedMe);
        WHEN(asedMe && !asedMe->isComplete()) .myExErr(me, ACCESS_TO_INCOMPLETE).ret();

        _STEP("check modifier.");
        if(!match->getModifier().isPublic()) { // we only need to run verify routine when there is
                                               // protected modifier.
            baseObj* castedMe = asedMe->cast<baseObj>();
            if(castedMe) { // if getExpr's castedMe is not derived one of baseObj, it's frame.
                const node& currentMe =
                    thread::get().getNowFrame() TO(getMe()) OR.myExErr(THERE_IS_NO_FRAMES_IN_THREAD).ret();
                WHEN(!castedMe->isSuper(currentMe)) .myExErr(me, CANT_ACCESS_TO_PROTECTED_VARIABLE, me.getName()).ret();
            }
        }
    }

    void me::onLeave(const visitInfo& i, retExpr& me, nbool) {
        _GUARD("onLeave(retExpr&)");

        _STEP("should be at last stmt");
        WHEN(i.index != i.len - 1) .myExErr(me, RET_AT_MIDDLE_OF_BLOCK).ret();

        _STEP("checks evalType of func is matched to me");
        const baseFunc& f = thread::get().getNowFrame() TO(getFunc()) OR.myExErr(me, NO_FUNC_INFO).ret();
        str myRet = me.getRet().infer() OR.myExErr(me, EXPR_EVAL_NUL).ret();

        const node& funRet = f.getRet() OR.myExErr(me, NO_RET_TYPE).ret();
        _STEP("checks return[%s] == func[%s]", myRet, funRet);

        WHEN(!myRet->isSub<baseErr>() && !myRet->isImpli(funRet)) .myExErr(me, RET_TYPE_NOT_MATCH, myRet, funRet).ret();
    }

    void me::onLeave(const visitInfo& i, runExpr& me, nbool) {
        _GUARD("onLeave(runExpr&)");

        _STEP("is it possible to run?");
        WHEN_NUL(me.getMe()).myExErr(me, DONT_KNOW_ME).ret();

        str ased = me.getMe() TO(infer()) OR.ret();
        args& a = me.getArgs();
        a.setMe(*ased);

        node& anySub = me.getSubj();
        _STEP("anySub[%s]", anySub);

        str derivedSub = anySub.infer() OR.myExErr(me, CANT_ACCESS, ased, "sub-node").ret();

        _STEP("derivedSub[%s]", derivedSub);
        if(!derivedSub->canRun(me.getArgs())) {
            const baseFunc* derivedCast = derivedSub->cast<baseFunc>();
            std::string params = !derivedCast ? "ctor" : _asStr(derivedCast->getParams());
            return BY_WHEN.myExErr(me, OBJ_WRONG_ARGS, i.name.c_str(), me.getArgs().asStr().c_str(), params.c_str())
                .ret();
        }

        a.setMe(nullptr);
    }

    void me::onTraverse(runExpr& me, node& subject) {
        str ased = me.getMe() TO(infer()) OR.ret();

        getExpr* cast = subject.cast<getExpr>();
        if(cast) cast->setMe(*ased);
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

    nbool me::onVisit(const visitInfo& i, ctor& me, nbool) {
        _GUARD("onVisit(ctor&)");
        WHEN_NUL(me.getRet()).myExErr(me, CTOR_NOT_IN_DEF_OBJ).ret(true);
        return onVisit(i, (ctor::super&) me, false);
    }

    void me::onLeave(const visitInfo& i, ctor& me, nbool) {
        _GUARD("onLeave(ctor&)");

        _STEP("no error allowed during running ctor");
        const node& eval = me.getBlock().infer().get() OR.myExErr(me, EXPR_EVAL_NUL).ret();
        WHEN(eval.isSub<baseErr>()) .myExErr(me, RET_ERR_ON_CTOR).ret();

        me.outFrame();
    }

    nbool me::onVisit(const visitInfo& i, func& me, nbool) {
        _GUARD("onVisit(func&)");

        onVisit(i, (func::super&) me, false);

        obj& meObj = thread::get()
                         ._getNowFrame() TO(getMe()) TO(template cast<obj>()) OR.myExErr(me, FUNC_REDIRECTED_OBJ)
                         .ret(true);

        _STEP("check func duplication");
        const nbicontainer& top = meObj.getShares().getContainer();
        ncnt len = me.getParams().len();
        const node* errFound = top.get([&](const std::string& key, const node& val) {
            WHEN(key != i.name) .ret(false);
            WHEN(&val == &me) .ret(false);

            // this has same name on shares, but it's not func! it's not valid.
            // this could be an origin obj.
            const baseFunc& cast = val.cast<baseFunc>() OR.ret(true);
            const params& castPs = cast.getParams();
            WHEN(castPs.len() != len) .ret(false);

            for(int n = 0; n < castPs.len(); n++) {
                str lhs = castPs[n].getOrigin().infer();
                str rhs = me.getParams()[n].getOrigin().infer();
                WHEN(lhs->getType() != rhs->getType()) .ret(false);
            }

            return true;
        });
        if(errFound) BY_WHEN.myExErr(me, ALREADY_DEFINED_FUNC, i.name.c_str());

        //  obj or property shouldn't have same name to any func.
        _STEP("check func has same name to field");
        if(meObj.getOwns().get(i.name) TO(template cast<baseObj>()))
            BY_WHEN.myExErr(me, ALREADY_DEFINED_IDENTIFIER, i.name.c_str());

        _STEP("main func return type should be int or void");
        if(i.name == starter::MAIN) {
            str ret = me.getRet();
            if(!ret->isSub<nInt>() && !ret->isSub<nVoid>()) BY_WHEN.myExErr(me, MAIN_FUNC_RET_TYPE);
        }

        _STEP("retType exists and stmts exist one at least");
        str retType = me.getRet() OR.myExErr(me, NO_RET_TYPE).ret(true);
        WHEN(!retType->isSub(ttype<node>::get())) .myExErr(me, WRONG_RET_TYPE, retType).ret(true);

        blockExpr& blk = me.getBlock();
        if(blk.getStmts().len() <= 0) {
            if(i.name == starter::MAIN) BY_WHEN.myExErr(blk, MAIN_SHOULD_HAVE_STMTS);
            /*TODO: uncomment after implement abstract:
            if(!func.isAbstract() && !retType->isSub<nVoid>())
                error(...)
            */
            return true;
        }

        _STEP("'break' or 'next' can't be used to last stmt");
        const node& lastStmt = *blk.getStmts().last();
        WHEN(lastStmt.isSub<retStateExpr>() && !lastStmt.isSub<retExpr>())
            .myExErr(lastStmt, FUNC_SHOULD_RETURN_SOMETHING).ret(true);

        _STEP("func[%s]: %s iterateBlock[%d]", i, me, me._blk->subs().len());

        // sequence of adding frame matters:
        //  object scope was added at 'onVisit(visitInfo, baseObj&)
        //  parameters of func is second:
        scope s;
        for(const auto& p: me.getParams()) {
            if(p.getOrigin().isSub<nVoid>()) {
                BY_WHEN.myExErr(me, PARAM_NOT_VOID, p.getName().c_str());
                continue;
            }
            str eval = p.getOrigin() TO(infer()) OR_CONTINUE;
            s.add(p.getName(), *new mockNode(*eval));
        }

        //  function's subs are third:
        me.inFrame(s.len() > 0 ? &s : nullptr);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, closure& me, nbool) {
        _GUARD("onVisit(closure&)");
        return true;
    }

    void me::onLeave(const visitInfo& i, closure& me, nbool) {
        _GUARD("onLeave(closure&)");
        // do nothing:
        //  if I don't override this, closure::outFrame will be called.
        //  I have overrided onVisit for clousre, so I override onLeave() too.
    }

    void me::_prepare() {
        super::_prepare();
        _recentLoops.clear();
    }

    void me::_onEndWork() {
        for(baseObj* org: _orgs)
            org->setState(VERIFIED);

        super::_onEndWork();
    }

    void me::onLeave(const visitInfo& i, func& me, nbool) {
        _GUARD("onLeave(func&)");

        _STEP("last stmt should match to ret type");
        BY_END(me.outFrame());
        const node& ret = me.getRet() OR.err("func.getRet() is null").ret();
        const type& retType = ret.getType();
        const node& lastStmt = *me.getBlock().getStmts().last();

        WHEN(retType == ttype<nVoid>::get()) .info("func: implicit return won't verify WHEN retType is void.").ret();

        str eval = me.getBlock().infer() OR.myExErr(lastStmt, NO_RET_TYPE).ret();

        // to get type of expr, always uses evalType.
        const ntype& lastType = eval->getType();

        _STEP("last stmt[%s] should matches to return type[%s]", eval, retType);

        // @see retExpr::infer()or more info.
        WHEN(eval->isSub<retStateExpr>()) .info("func: skip verification WHEN lastStmt is retStateExpr.").ret();
        WHEN(!lastType.isSub<baseErr>() && !lastType.isImpli(retType))
            // TODO: remove .myExErr(lastStmt ? me : lastStmt, RET_TYPE_NOT_MATCH, lastType,
            // retType)
            .myExErr(me, RET_TYPE_NOT_MATCH, lastType, retType).ret();
    }

    nbool me::onVisit(const visitInfo& i, baseObj& me, nbool) {
        _GUARD("onVisit(baseObj&)");

        me.inFrame();
        frame& fr = thread::get()._getNowFrame() OR.myExErr(THERE_IS_NO_FRAMES_IN_THREAD).ret(true);
        if(!me.isComplete()) {
            fr.subs().del("me");
            fr.subs().add("me", new mockNode(me));
        }

        _STEP("%s push me[%s] len=%d", fr.getMe(), (void*) fr.getMe(), me.subs().len());

        _STEP("iterate all subs and checks void type variable");
        for(const node& elem: me.subs())
            if(elem.isSub<nVoid>()) BY_WHEN.myExErr(elem, VOID_CANT_DEFINED);

        _STEP("did user set the name of this object like 'const'?");
        WHEN(util::checkTypeAttr(i.name) == ATTR_CONST && i.name.length() > 1)
            .myExErr(me, ORIGIN_OBJ_CANT_BE_CONST).ret(true);

        // TODO: remove _STEP("origin obj always must exist");
        // me.getOrigin() OR.myExErr(me, NO_ORIGIN, i.name).ret(false);

        onLeave(i, (baseObj::super&) me, false);
        return true;
    }

    void me::onLeave(const visitInfo& i, baseObj& me, nbool) {
        _GUARD("onLeave(baseObj&)");
        me.outFrame();
        _orgs.push_back(&me);
    }

    nbool me::onVisit(const visitInfo& i, genericOrigin& me, nbool) {
        _GUARD("onVisit(genericOrigin&)");

        _STEP("cache check");
        for(const auto& e: me._cache)
            if(!e.second) BY_WHEN.myExErr(me, MAKE_GENERIC_FAIL, e.first.c_str());

        _STEP("did user set the name of this object like 'const'?");
        WHEN(util::checkTypeAttr(i.name) == ATTR_CONST && i.name.length() > 1)
            .myExErr(me, ORIGIN_OBJ_CANT_BE_CONST, me).ret(true);

        _STEP("if obj is complete, does it have ctor without params?");
        if(me.isComplete()) WHEN_NUL(me.sub(ctor::CTOR_NAME, args())).myExErr(me, COMPLETE_OBJ_BUT_NO_CTOR).ret(true);

        return true;
    }

    void me::onLeave(const visitInfo& i, genericOrigin& me, nbool) {
        _GUARD("onLeave(genericOrigin&)");

        _orgs.push_back(&me);
        // DON'T CALL 'super::onLeave()':
        //  if I don't have this func, getGenericExpr::super (=baseObj)'s one will be called.
        //  and me pointer will be erased too inside the func.
    }

    nbool me::onVisit(const visitInfo& i, forExpr& me, nbool) {
        _GUARD("onVisit(forExpr&)");
        _recentLoops.push_back(&me);

        str container = me._container;
        str conAsed = container->infer() OR.myExErr(me, CONTAINER_IS_NUL).ret(true);
        str elemType = conAsed->run("getElemType") OR.myExErr(me, ELEM_TYPE_IS_NUL).ret(true);

        const std::string& name = me.getLocalName();
        _STEP("define iterator '%s %s'", elemType, name);

        scope* s = new scope();
        s->add(name, *((node*) elemType->clone()));

        thread::get()._getNowFrame() TO(add(*s));
        return true;
    }

    void me::onLeave(const visitInfo& i, forExpr& me, nbool) {
        _GUARD("onLeave(forExpr&)");

        thread::get()._getNowFrame() TO(del()); // for a scope containing iterator.
        _onLeave(i, me);
    }

    nbool me::onVisit(const visitInfo& i, whileExpr& me, nbool) {
        _GUARD("onLeave(whileExpr&)");
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(const visitInfo& i, breakExpr& me, nbool) {
        _GUARD("onLeave(whileExpr&)");

        _STEP("declared outside of loop?");
        WHEN(_recentLoops.size() <= 0) .myExErr(me, BREAK_OUTSIDE_OF_LOOP).ret();
    }

    void me::onLeave(const visitInfo& i, nextExpr& me, nbool) {
        _GUARD("onLeave(nextExpr&)");

        _STEP("declared outside of loop?");
        WHEN(_recentLoops.size() <= 0) .myExErr(me, NEXT_OUTSIDE_OF_LOOP).ret();
    }

    nbool me::onVisit(const visitInfo& i, ifExpr& me, nbool) {
        _GUARD("onVisit(ifExpr&)");
        return true;
    }

    void me::onLeave(const visitInfo& i, ifExpr& me, nbool) { _GUARD("onLeave(ifExpr&)"); }

#undef _GUARD
#undef _STEP

} // namespace by
