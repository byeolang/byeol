#include "verifier.hpp"

#include "../../ast.hpp"
#include "../../builtin/primitive.hpp"
#include "../../frame/frame.hpp"
#include "../../frame/starter.hpp"
#include "../../frame/thread.hpp"
#include "../../builtin/err/errReport.hpp"
#include "../worker.inl"
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

#define thenErr_1(code) exErr(errCode::code, getReport())
#define thenErr_2(me, code) exErr((me).getSrc().getPos(), errCode::code, getReport())
#define thenErr_3(me, code, a1) exErr((me).getSrc().getPos(), errCode::code, getReport(), a1)
#define thenErr_4(me, code, a1, a2) \
    exErr((me).getSrc().getPos(), errCode::code, getReport(), a1, a2)
#define thenErr_5(me, code, a1, a2, a3) \
    exErr((me).getSrc().getPos(), errCode::code, getReport(), a1, a2, a3)
#define thenErr(...) NM_OVERLOAD(thenErr, __VA_ARGS__)

#define thenWarn_1(code) exErr(errCode::code, getReport())
#define thenWarn_2(me, code) exErr((me).getSrc().getPos(), errCode::code, getReport())
#define thenWarn_3(me, code, a1) exErr((me).getSrc().getPos(), errCode::code, getReport(), a1)
#define thenWarn_4(me, code, a1, a2) \
    exErr((me).getSrc().getPos(), errCode::code, getReport(), a1, a2)
#define thenWarn_5(me, code, a1, a2, a3) \
    exErr((me).getSrc().getPos(), errCode::code, getReport(), a1, a2, a3)
#define thenWarn(...) NM_OVERLOAD(thenWarn, __VA_ARGS__)

#define orRetErr(...) orRet posError(__VA_ARGS__)

#define _GUARD(msg)                                         \
    if(isFlag(GUARD)) do {                                  \
            NM_I("'%s' %s@%s: " msg, i, me.getType(), &me); \
            _stepN = 0;                                     \
    } while(0)

#define _STEP(msg, ...)                                                                      \
    NM_I("'%s' %s@%s: step#%d --> " msg, i, ttype<typeTrait<decltype(me)>::Org>::get(), &me, \
        ++_stepN, ##__VA_ARGS__)

    // verification:
    void me::onLeave(const visitInfo& i, node& me, nbool) {
        _GUARD("onLeave(node&)");

        _STEP("no same variable=%d", me.subs().len());
        if(me.isSub<frame>()) return;

        for(auto e = me.subs().begin(); e; ++e) {
            auto matches = me.subAll<baseObj>(e.getKey());

            WHEN(matches.len() > 1).thenErr(*e, DUP_VAR, e.getKey()).ret();
        }
    }

    void me::onLeave(const visitInfo& i, asExpr& me, nbool) {
        _GUARD("onLeave(asExpr&)");

        _STEP("_me & _as aren't null");
        WHEN_NUL(me.getMe()).thenErr(me, LHS_IS_NUL).ret();
        WHEN_NUL(me.getAs()).thenErr(me, RHS_IS_NUL).ret();
        WHEN(me.getAs().isSub<nVoid>()).thenErr(me, VOID_NOT_CAST).ret();

        _STEP("checks that me can cast to 'as'");
        WHEN(!me.getMe().is(me.getAs()))
            .thenErr(me, CAST_NOT_AVAILABLE, me.getMe(), me.getAs())
            .ret();

        _STEP("rhs shouldn't be expression");
        WHEN(!me.getAs().isImpli<node>()).thenErr(me, CAST_TO_UNKNOWN).ret();
    }

    void me::onLeave(const visitInfo& i, isExpr& me, nbool) {
        _GUARD("onLeave(isExpr&)");

        _STEP("_me & _to aren't null");
        WHEN_NUL(me.getMe()).thenErr(me, LHS_IS_NUL).ret();
        WHEN_NUL(me.getTo()).thenErr(me, RHS_IS_NUL).ret();

        _STEP("checks that me can cast to 'as'");
        WHEN(!me.getMe().is(me.getTo()))
            .thenErr(me, CAST_NOT_AVAILABLE, me.getMe(), me.getTo())
            .ret();

        _STEP("rhs shouldn't be expression");
        WHEN(!me.getTo().isImpli<node>()).thenErr(me, CAST_TO_UNKNOWN).ret();
    }

    void me::onLeave(const visitInfo& i, assignExpr& me, nbool) {
        _GUARD("onLeave(assignExpr&)");

        _STEP("set evalType");

        const ntype& ltype = me TO(getLeft()) TO(getEval()) TO(getType()) orRetErr(LHS_IS_NUL, me);
        const ntype& rtype = me TO(getRight()) TO(getEval()) TO(getType()) orRetErr(RHS_IS_NUL, me);
        WHEN(rtype.isSub<retStateExpr>()).thenErr(me, CANT_ASSIGN_RET).ret();
        WHEN(!rtype.isImpli(ltype)).thenErr(me, TYPE_NOT_COMPATIBLE, rtype, ltype).ret();

        // verify rvalue and lvalue:
        //  first of all:
        //      in namulang there is no lvalue concept. to involve lvalue concept,
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
        const node& lhs = me.getLeft();
        const getExpr& e = lhs.cast<getExpr>();
        WHEN_NUL(e /*TODO: && !lhs.isSub<ElementExpr>()*/)
            .thenErr(me, ASSIGN_TO_RVALUE, me.getRight(), lhs)
            .ret();

        _STEP("checks that you can't assign to a func");
        str lhsEval = e.getEval();
        WHEN(baseFunc::isFuncButNotClosure(*lhsEval)).thenErr(me, ASSIGN_TO_FUNC).ret();

        _STEP("checks that try to assign to a const variable");
        const getExpr& leftCast = me.getLeft().cast<getExpr>();
        WHEN(util::checkTypeAttr(leftCast.getName()) == ATTR_CONST)
            .thenErr(me, ASSIGN_TO_CONST, leftCast.getName())
            .ret();
    }

    nbool me::onVisit(const visitInfo& i, blockExpr& me, nbool) {
        _GUARD("onVisit(blockExpr&)");

        me.inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, blockExpr& me, nbool) {
        _GUARD("onLeave(blockExpr&)");

        const narr& stmts = me.getStmts();
        if(nul(stmts) || stmts.len() <= 0) return; // will be catched to another verification.

        me.setEval(*me.getEval());
        me.outFrame();
    }

    void me::_onLeave(const visitInfo& i, const loopExpr& me) {
        _recentLoops.pop_back();

        str eval = me.getEval(); // it's okay forExpr not to have 'eval'.
        if(eval) {
            _STEP("eval Value check: eval[%s] is an array?", eval);
            WHEN(!eval->isSub<retStateExpr>() && !eval->isSub<arr>())
                .thenErr(me, LOOP_NO_RET_ARR)
                .ret();
        }
    }

    void me::onLeave(const visitInfo& i, defVarExpr& me, nbool) {
        _GUARD("onLeave(defVarExpr&)");

        _STEP("modifier not allowed for local variables in a func.");
        const auto& mod = me.getNewModifier() orRetErr(MODIFIER_NOT_FOUND, me, me.getName());
        frame& fr = thread::get()._getNowFrame() orRetErr(THERE_IS_NO_FRAMES_IN_THREAD, me);
        const baseFunc& fun = fr.getFunc();
        nbool isInLocal = nul(fun) || fun.getSrc().getName() != baseObj::EXPAND_NAME;
        WHEN(isInLocal && !mod.isPublic())
            .thenErr(me, PROTECTED_NOT_ALLOWED_FOR_LOCAL, me.getName())
            .ret();
        WHEN(mod.isExplicitOverride())
            .thenErr(me, OVERRIDE_NOT_ALLOWED_FOR_LOCAL, me.getName())
            .ret();

        _STEP("to define a void type property isn't allowed.");
        str eval = me.getRight() TO(getEval()) orRetErr(RHS_IS_NUL, me);
        WHEN(eval->isSub<nVoid>()).thenErr(me, VOID_CANT_DEFINED).ret();

        obj& cast = eval->cast<obj>();
        if(!nul(cast)) {
            const baseObj& org = cast.getOrigin();
            WHEN(org.getState() < PARSED)
                .thenErr(me, TYPE_IS_NOT_PRE_EVALUATED, me.getName())
                .ret();
        }

        _STEP("does rhs[%s] have 'ret' in its blockStmt?", eval);
        WHEN(eval->isSub<retStateExpr>()).thenErr(me, CANT_ASSIGN_RET).ret();

        _STEP("check whether make a void container.");
        const auto& ps = eval->getType().getParams();
        for(const auto& p: ps)
            WHEN(p.getOrigin().isSub<nVoid>()).thenErr(me, NO_VOID_CONTAINER).ret();

        std::string name = me.getName();
        WHEN(name == "").thenErr(me, HAS_NO_NAME).ret();

        _STEP("is %s definable?", name);
        const ntype& t = eval->getType();
        if(nul(t)) posError(errCode::CANT_DEF_VAR, me, name.c_str(), "null");
        WHEN(eval->isSub<nVoid>()).thenErr(me, VOID_CANT_DEFINED).ret();

        node& to = me.getTo();
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
        if(!nul(to)) return;

        _STEP("duplication of variable with name[%s]", name);
        WHEN(fr.mySubs()->in(name)).thenErr(me, ALREADY_DEFINED_VAR, name, t).ret();

        _STEP("ok. defining local temporary var... %s %s", name, t);
        fr.addLocal(name, *new mockNode(*eval));
    }

    void me::onLeave(const visitInfo& i, defPropExpr& me, nbool) {
        _GUARD("onLeave(defPropExpr&)");

        _STEP("whether the 'type' object has a ctor without any paramters?");
        str eval = me TO(getRight()) TO(getEval()) orRetErr(RHS_IS_NUL, me);
        WHEN(eval->isSub<baseObj>() && nul(eval->sub(baseObj::CTOR_NAME, args{})))
            .thenErr(me, DONT_HAVE_CTOR, eval)
            .ret();
        func& fun = eval->cast<func>();
        WHEN(!nul(fun) && fun.isAbstract())
            .thenErr(me, YOU_CANT_DEFINE_PROPERTY_WITH_ABSTRACT_FUNC)
            .ret();

        onLeave(i, (defPropExpr::super&) me, false);
    }

    void me::onLeave(const visitInfo& i, defAssignExpr& me, nbool) {
        _GUARD("onLeave(defAssignExpr&)");

        _STEP("check rhs");
        str eval = me TO(getRight()) TO(getEval()) orRetErr(RHS_IS_NUL, me);
        WHEN(!eval->isComplete()).thenErr(me, ACCESS_TO_INCOMPLETE).ret();

        str explicitType = me TO(getExplicitType());
        if(explicitType) {
            _STEP("check explicit type whether it's valid");
            str type = me TO(getEval());
            WHEN_NUL(type).thenErr(me, EXPLICIT_TYPE_SHOULDNT_BE_NULL).ret();
            WHEN(type->isSub<nVoid>()).thenErr(me, NO_VOID_VARIABLE).ret();
        }

        onLeave(i, (defAssignExpr::super&) me, false);
    }

    void me::onLeave(const visitInfo& i, defSeqExpr& me, nbool) {
        _GUARD("onLeave(defSeqExpr&)");

        _STEP("check lhs & rhs");
        auto& start = me.getStart() orRetErr(LHS_IS_NUL, me);
        auto& end = me.getEnd() orRetErr(RHS_IS_NUL, me);

        _STEP("lhs & rhs is sort of Int?");
        WHEN(!start.isImpli<nInt>()).thenErr(me, SEQ_SHOULD_INT_COMPATIBLE).ret();
        WHEN(!end.isImpli<nInt>()).thenErr(me, SEQ_SHOULD_INT_COMPATIBLE).ret();
    }

    void me::onLeave(const visitInfo& i, defArrayExpr& me, nbool) {
        _GUARD("onLeave(defArrayExpr&)");

        _STEP("check all elements");
        const node& type = me.getArrayType() orRetErr(ELEM_TYPE_DEDUCED_NUL, me);
        WHEN(type.isSuper<obj>()).thenErr(me, ELEM_TYPE_DEDUCED_WRONG, type).ret();
        WHEN(type.isSub<nVoid>()).thenErr(me, ELEM_TYPE_NOT_VOID).ret();

        _STEP("check arr has exactly 1 type parameter.");
        const auto& ps =
            me.getOrigin() TO(getType()) TO(getParams()) orRetErr(ELEM_TYPE_IS_NUL, me);
        WHEN(ps.len() != 1).thenErr(me, ARR_DOESNT_HAVE_TYPE_PARAM).ret();
    }

    void me::onLeave(const visitInfo& i, defNestedFuncExpr& me, nbool) {
        _GUARD("onLeave(defNestedFuncExpr&)");

        _STEP("check me has origin");
        WHEN_NUL(me.getOrigin()).thenErr(me, NO_FUNC_INFO).ret();

        _STEP("does it have `eval`?");
        frame& fr = thread::get()._getNowFrame();
        WHEN_NUL(fr).thenErr(me, THERE_IS_NO_FRAMES_IN_THREAD).ret();
        str eval = me.getEval();
        WHEN_NUL(eval).thenErr(me, EXPR_EVAL_NUL).ret();

        _STEP("does it have a name?");
        const std::string& name = me.getOrigin().getSrc().getName();
        WHEN_NUL(name).thenErr(me, HAS_NO_NAME).ret();

        _STEP("define nested `%s` func in local", name);
        fr.addLocal(name, *eval);
    }

    void me::onLeave(const visitInfo& i, FBOExpr& me, nbool) {
        _GUARD("onLeave(FBOEXpr&)");

        _STEP("finding eval of l(r)hs.");
        str lEval = me TO(getLeft()) TO(getEval()) orRetErr(LHS_IS_NUL, me);
        str rEval = me TO(getRight()) TO(getEval()) orRetErr(RHS_IS_NUL, me);

        WHEN(!checkEvalType(*lEval)).thenErr(me, LHS_IS_NOT_ARITH, lEval).ret();
        WHEN(!checkEvalType(*rEval)).thenErr(me, RHS_IS_NOT_ARITH, rEval).ret();

        WHEN_NUL(lEval->deduce(*rEval))
            .thenErr(me, IMPLICIT_CAST_NOT_AVAILABLE, lEval, rEval)
            .ret();

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

    void me::onLeave(const visitInfo& i, FUOExpr& me, nbool) {
        _GUARD("onLeave(FUOExpr&)");

        _STEP("string isn't proper to any FUO operator");
        str eval = me.getEval();
        WHEN(eval && eval->isImpli<nStr>()).thenErr(me, STRING_IS_NOT_PROPER_TO_OP).ret();
    }

    void me::onLeave(const visitInfo& i, getExpr& me, nbool) {
        _GUARD("onLeave(getExpr&)");

        // TODO: I have to check that the evalType has what matched to given _params.
        // Until then, I rather use as() func and it makes slow emmersively.
        _STEP("isRunnable: %s.%s", me, me.getName());
        WHEN(!me.getEval()).thenErr(me, WHAT_IS_THIS_IDENTIFIER, me.getName()).ret();
        str match = me._get(true);
        if(!match) {
            const node& from = me.getMe();
            return posError(errCode::CANT_ACCESS, me, me._name.c_str(),
                from.getType().getName().c_str());
        }
        // TODO: leave logs for all ambigious candidates as err.
        WHEN_NUL(*match).thenErr(me, AMBIGIOUS_ACCESS, i).ret();

        _STEP("isRunnable: got=%s, me=%s", match, me.getType());

        str asedMe = me.getMe().getEval();
        _STEP("accesses to incomplete 'me[%s]' object?", asedMe);
        WHEN(asedMe && !asedMe->isComplete()).thenErr(me, ACCESS_TO_INCOMPLETE).ret();

        _STEP("check modifier.");
        const auto& mod = match->getModifier() orRetErr(MODIFIER_NOT_FOUND, me, me.getName());
        if(!mod.isPublic()) { // we only need to run verify routine when there is protected
                              // modifier.
            baseObj& castedMe = asedMe->cast<baseObj>();
            if(!nul(castedMe)) { // if getExpr's castedMe is not derived one of baseObj, it's frame.
                const node& currentMe = thread::get().getNowFrame().getMe();
                WHEN(!castedMe.isSuper(currentMe))
                    .thenErr(me, CANT_ACCESS_TO_PROTECTED_VARIABLE, me.getName())
                    .ret();
            }
        }
    }

    void me::onLeave(const visitInfo& i, retExpr& me, nbool) {
        _GUARD("onLeave(retExpr&)");

        _STEP("should be at last stmt");
        WHEN(i.index != i.len - 1).thenErr(me, RET_AT_MIDDLE_OF_BLOCK).ret();

        _STEP("checks evalType of func is matched to me");
        const baseFunc& f = thread::get().getNowFrame().getFunc() orRetErr(NO_FUNC_INFO, me);

        str myRet = me.getRet().getEval() orRetErr(EXPR_EVAL_NUL, me);

        const node& funRet = f.getRet();
        _STEP("checks return[%s] == func[%s]", myRet, funRet);

        WHEN(!myRet->isSub<baseErr>() && !myRet->isImpli(funRet))
            .thenErr(me, RET_TYPE_NOT_MATCH, myRet, funRet)
            .ret();
    }

    void me::onLeave(const visitInfo& i, runExpr& me, nbool) {
        _GUARD("onLeave(runExpr&)");

        _STEP("is it possible to run?");
        WHEN_NUL(me.getMe()).thenErr(me, DONT_KNOW_ME).ret();

        str ased = me.getMe().getEval() orRet;
        args& a = me.getArgs();
        a.setMe(*ased);

        node& anySub = me.getSubj() orRetErr(FUNC_NOT_EXIST, me);
        _STEP("anySub[%s]", anySub);

        str derivedSub = anySub.getEval() orRetErr(CANT_ACCESS, me, ased, "sub-node");

        _STEP("derivedSub[%s]", derivedSub);
        if(!derivedSub->canRun(me.getArgs())) {
            const baseFunc& derivedCast = derivedSub->cast<baseFunc>();
            std::string params = nul(derivedCast) ? "ctor" : _asStr(derivedCast.getParams());
            return posError(errCode::OBJ_WRONG_ARGS, me, i.name.c_str(),
                me.getArgs().asStr().c_str(), params.c_str());
        }

        a.setMe(nulOf<node>());
    }

    void me::onTraverse(runExpr& me, node& subject) {
        str ased = me.getMe() TO(getEval()) orRet;

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

    nbool me::onVisit(const visitInfo& i, ctor& me, nbool) {
        _GUARD("onVisit(ctor&)");
        WHEN_NUL(me.getRet()).thenErr(me, CTOR_NOT_IN_DEF_OBJ).ret(true);
        return onVisit(i, (ctor::super&) me, false);
    }

    void me::onLeave(const visitInfo& i, ctor& me, nbool) {
        _GUARD("onLeave(ctor&)");

        _STEP("no error allowed during running ctor");
        const node& eval = *me.getBlock().getEval() orRetErr(EXPR_EVAL_NUL, me);
        WHEN(eval.isSub<baseErr>()).thenErr(me, RET_ERR_ON_CTOR).ret();

        me.outFrame(scope());
    }

    nbool me::onVisit(const visitInfo& i, func& me, nbool) {
        _GUARD("onVisit(func&)");

        onVisit(i, (func::super&) me, false);

        obj& meObj = thread::get()._getNowFrame() TO(getMe()) TO(template cast<obj>());
        WHEN_NUL(meObj).thenErr(me, FUNC_REDIRECTED_OBJ).ret(true);

        _STEP("check func duplication");
        const nbicontainer& top = meObj.getShares().getContainer();
        ncnt len = me.getParams().len();
        const node& errFound = top.get([&](const std::string& key, const node& val) {
            if(key != i.name) return false;
            if(&val == &me) return false;

            // this has same name on shares, but it's not func! it's not valid.
            // this could be an origin obj.
            const baseFunc& cast = val.cast<baseFunc>() orRet true;
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
        if(!nul(meObj.getOwns().get(i.name).cast<baseObj>()))
            posError(errCode::ALREADY_DEFINED_IDENTIFIER, me, i.name.c_str());

        _STEP("main func return type should be int or void");
        if(i.name == starter::MAIN) {
            str ret = me.getRet();
            if(!ret->isSub<nInt>() && !ret->isSub<nVoid>())
                posError(errCode::MAIN_FUNC_RET_TYPE, me);
        }

        _STEP("retType exists and stmts exist one at least");
        str retType = me.getRet() orRet posError(errCode::NO_RET_TYPE, me), true;
        WHEN(!retType->isSub(ttype<node>::get())).thenErr(me, WRONG_RET_TYPE, retType).ret(true);

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
        WHEN(lastStmt.isSub<retStateExpr>() && !lastStmt.isSub<retExpr>())
            .thenErr(lastStmt, FUNC_SHOULD_RETURN_SOMETHING)
            .ret(true);

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
            str eval = p TO(getOrigin()) TO(getEval()) orContinue;
            s->add(p.getName(), *new mockNode(*eval));
        }

        //  function's subs are third:
        me.inFrame(*s);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, closure& me, nbool) {
        _GUARD("onVisit(closure&)");

        _STEP("is me captured?");
        WHEN_NUL(me.getOrigin()).thenErr(me, NOT_CAPTURED).ret(true);

        _STEP("does it have a func info?");
        WHEN_NUL(me.getFunc()).thenErr(me, NOT_HAS_FUNC).ret(true);
        return true;
    }

    void me::onLeave(const visitInfo& i, closure& me, nbool) {
        _GUARD("onLeave(closure&)");
        // do tnohing:
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
        NM_END(me.outFrame(scope()));
        const type& retType = me.getRet() TO(getType()) orRet NM_E("func.getRet() is null");
        const node& lastStmt = *me.getBlock().getStmts().last();

        if(retType == ttype<nVoid>::get())
            return NM_I("func: implicit return won't verify WHEN retType is void."), void();

        str eval = me.getBlock().getEval() orRetErr(NO_RET_TYPE, lastStmt);

        // to get type of expr, always uses evalType.
        const ntype& lastType = eval->getType() orRetErr(NO_RET_TYPE, lastStmt);

        _STEP("last stmt[%s] should matches to return type[%s]", eval, retType);
        if(eval->isSub<retStateExpr>())
            // @see retExpr::getEval() for more info.
            return NM_I("func: skip verification WHEN lastStmt is retStateExpr."), void();
        WHEN(!lastType.isSub<baseErr>() && !lastType.isImpli(retType))
            .thenErr(nul(lastStmt) ? me : lastStmt, RET_TYPE_NOT_MATCH, lastType, retType)
            .ret();
    }

    nbool me::onVisit(const visitInfo& i, baseObj& me, nbool) {
        _GUARD("onVisit(baseObj&)");

        me.inFrame();
        frame& fr = thread::get()._getNowFrame();
        if(!me.isComplete()) {
            fr.subs().del("me");
            fr.subs().add("me", new mockNode(me));
        }

        _STEP("%s push me[%s] len=%d", fr.getMe(), (void*) &fr.getMe(), me.subs().len());

        _STEP("iterate all subs and checks void type variable");
        for(const node& elem: me.subs())
            if(elem.isSub<nVoid>()) posError(errCode::VOID_CANT_DEFINED, elem);

        _STEP("did user set the name of this object like 'const'?");
        WHEN(util::checkTypeAttr(i.name) == ATTR_CONST && i.name.length() > 1)
            .thenErr(me, ORIGIN_OBJ_CANT_BE_CONST)
            .ret(true);

        _STEP("origin obj always must exist");
        me.getOrigin() orRetErr(NO_ORIGIN, me, i.name), false;

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
            if(nul(e.second)) posError(errCode::MAKE_GENERIC_FAIL, me, e.first.c_str());

        _STEP("did user set the name of this object like 'const'?");
        WHEN(util::checkTypeAttr(i.name) == ATTR_CONST && i.name.length() > 1)
            .thenErr(me, ORIGIN_OBJ_CANT_BE_CONST, me)
            .ret(true);

        _STEP("if obj is complete, does it have ctor without params?");
        if(me.isComplete())
            WHEN_NUL(me.sub(baseObj::CTOR_NAME, args{}))
                .thenErr(me, COMPLETE_OBJ_BUT_NO_CTOR)
                .ret(true);

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
        str conAsed = container->getEval() orRetErr(CONTAINER_IS_NUL, me), true;
        str elemType = conAsed->run("getElemType") orRetErr(ELEM_TYPE_IS_NUL, me), true;

        const std::string& name = me.getLocalName();
        _STEP("define iterator '%s %s'", elemType, name);

        scope* s = new scope();
        s->add(name, *((node*) elemType->clone()));

        thread::get()._getNowFrame().add(*s);
        return true;
    }

    void me::onLeave(const visitInfo& i, forExpr& me, nbool) {
        _GUARD("onLeave(forExpr&)");

        thread::get()._getNowFrame().del(); // for a scope containing iterator.
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
        WHEN(_recentLoops.size() <= 0).thenErr(me, BREAK_OUTSIDE_OF_LOOP).ret();
    }

    void me::onLeave(const visitInfo& i, nextExpr& me, nbool) {
        _GUARD("onLeave(nextExpr&)");

        _STEP("declared outside of loop?");
        WHEN(_recentLoops.size() <= 0).thenErr(me, NEXT_OUTSIDE_OF_LOOP).ret();
    }

    nbool me::onVisit(const visitInfo& i, ifExpr& me, nbool) {
        _GUARD("onVisit(ifExpr&)");
        return true;
    }

    void me::onLeave(const visitInfo& i, ifExpr& me, nbool) { _GUARD("onLeave(ifExpr&)"); }

#undef thenErr
#undef thenWarn
#undef orRetErr
} // namespace nm
