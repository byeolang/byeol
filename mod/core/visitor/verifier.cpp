#include "verifier.hpp"
#include "../frame/frame.hpp"
#include "../frame/thread.hpp"
#include "../loader/errReport.hpp"
#include "../builtin/primitive.hpp"
#include "../ast.hpp"
#include "../frame/starter.hpp"
#include "../loader/worker/worker.inl"

namespace namu {

    NAMU_DEF_ME(verifier)

#define GUARD(...) if(isFlag(GUARD)) NAMU_I(__VA_ARGS__)

    namespace {
        str primitives[] = {
            new nInt(),
            new nFlt(),
            new nStr(),
            new nByte(),
            new nChar(),
            new nBool(),
        };

        nbool checkEvalType(const node& eval) {
            if(nul(eval)) return false;
            for(str e : primitives)
                if(eval.isSub(*e))
                    return true;

            return false;
        }
    }

    void me::_onWork() {
        if(nul(thread::get())) {
            error(errCode::NO_THREAD);
            return;
        }

        super::_onWork();
    }

    void me::_onEndWork() {
        _postpare();
    }

    // verification:
    nbool me::onVisit(visitInfo i, node& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: node: no same variable=%d", me.subs().len());
        if(me.isSub<frame>()) return true;

        for(auto e=me.subs().begin(); e ;++e) {
            auto matches = me.subAll<baseObj>(e.getKey()).getMatches();
            if(matches.len() > 1)
                return posError(errCode::DUP_VAR, *e, e.getKey().c_str()), true;
        }
        return true;
    }

    nbool me::onVisit(visitInfo i, asExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: asExpr: _me & _as aren't null");
        if(nul(me.getMe())) return posError(errCode::LHS_IS_NULL, me), true;
        if(nul(me.getAs())) return posError(errCode::RHS_IS_NULL, me), true;
        if(me.getAs().isSub<nVoid>()) return posError(errCode::VOID_NOT_CAST, me), true;

        NAMU_I("verify: asExpr: checks that me can cast to 'as'");
        if(!me.getMe().is(me.getAs()))
            return posError(errCode::CAST_NOT_AVAILABLE, me, me.getMe().getType().getName().c_str(),
                    me.getAs().getType().getName().c_str()), true;

        NAMU_I("verify: asExpr: rhs shouldn't be expression");
        if(!me.getAs().asImpli<node>())
            return posError(errCode::CAST_TO_UNKNOWN, me), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, assignExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: assignExpr: set evalType");
        str leftEval = me.getLeft().getEval();
        if(!leftEval) return posError(errCode::LHS_IS_NULL, me), true;
        const ntype& ltype = leftEval->getType();
        if(nul(ltype)) return posError(errCode::LHS_IS_NULL, me), true;
        str rightEval = me.getRight().getEval();
        if(!rightEval) return posError(errCode::RHS_IS_NULL, me), true;
        const ntype& rtype = rightEval->getType();
        if(nul(rtype)) return posError(errCode::RHS_IS_NULL, me), true;
        if(!rtype.isImpli(ltype))
            return posError(errCode::TYPE_NOT_COMPATIBLE, me, rtype.getName().c_str(), ltype.getName()
                    .c_str()), true;

        // verify rvalue and lvalue:
        //  first of all:
        //      in namulang there is no lvalue concept. to involve lvalue concept,
        //      I need to bring new concept or feature like 'const' but I don't want to.
        //      because the direction of this language is a toy lang. DON'T be serious.
        //      but I can't leave something which is going to operate like a lvalue by user.
        //      so I'll emulate the way how lvalue works.
        //
        //  when you assign something:
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
        //          when verifier meets ElementExpr, it doesn't need to deep search any longer to
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
        NAMU_I("verify: assignExpr: checks rvalue");
        const node& lhs = me.getLeft();
        if(!lhs.isSub<getExpr>()/* TODO: && !lhs.isSub<ElementExpr>()*/)
            return posError(errCode::ASSIGN_TO_RVALUE, me, me.getRight().getType().getName().c_str(),
                    lhs.getType().getName().c_str()), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, blockExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: blockExpr: it will iterate all subnodes[%d]", me.getStmts().len());
        return true;
    }

    void me::onLeave(visitInfo i, blockExpr& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: blockExpr: last stmt should match to ret type");
        const narr& stmts = me.getStmts();
        if(nul(stmts) || stmts.len() <= 0) return; // will be catched to another verification.

        func& parent = i.parent ? i.parent->cast<func>() : nulOf<func>();
        if(!nul(parent))
            _verifyMgdFuncImplicitReturn(parent);
    }

    nbool me::_isVariableDuplicated(defAssignExpr& me, const node& it) {
        NAMU_I("verify: defAssignExpr: duplication of variable with name[%s] at %s obj", me.getSubName().c_str(), it.getType().getName().c_str());
        if(it.mySubs()->has(me.getSubName()))
            return posError(errCode::ALREADY_DEFINED_VAR, me, me.getSubName().c_str(), me.getEval()->getType().getName()
                    .c_str()), true;
        return false;
    }

    void me::onLeave(visitInfo i, defAssignExpr& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: defAssignExpr: is definable?");
        const node& rhs = me.getRight();
        if(nul(rhs))
            return posError(errCode::CANT_DEF_VAR, me, me.getSubName().c_str(), "null");

        NAMU_I("verify: defAssignExpr: '%s %s' has defined.", me.getSubName().c_str(),
                nul(rhs) ? "name" : rhs.getType().getName().c_str());

        node& to = me.getTo();
        str new1 = me.isOnDefBlock() ? rhs.as<node>() : rhs.getEval();

        NAMU_I("verify: defAssignExpr: define new1[%s] to[%s]",
               new1 ? new1->getType().getName().c_str() : "null", nul(to) ? "frame" : to.getType().getName().c_str());
        if(!new1)
            return posError(errCode::RHS_NOT_EVALUATED, me);
        if(!new1->isComplete())
            return posError(errCode::ACCESS_TO_INCOMPLETE, me);
        if(new1->isSub<nVoid>())
            return posError(errCode::VOID_CANT_DEFINED, me);

        // when you define variable, I need to clone it:
        //  if don't, it may be incomplete object.
        if(nul(to)) {
            frame& fr = thread::get()._getNowFrame();
            scope& sc = (scope&) fr.subs();
            if(sc.getContainer().has(me.getSubName()))
                return posError(errCode::ALREADY_DEFINED_VAR, me, me.getSubName().c_str(),
                        rhs.getType().getName().c_str());
            if(_isVariableDuplicated(me, fr)) return;
            fr.addLocal(me.getSubName(), *new1);
            NAMU_I("verify: defAssignExpr: define new variable '%s %s' at frame", me.getSubName().c_str(), new1->getType().getName().c_str());
        } else {
            str dest = to.run();
            if(!dest)
                return posError(errCode::EXPR_EVAL_NULL, me);
            auto& subs = dest->subs();
            if(_isVariableDuplicated(me, *dest)) return;
            subs.add(me.getSubName(), *new1);
            NAMU_I("verify: defAssignExpr: define new variable '%s %s' at %s",
                   me.getSubName().c_str(), new1->getType().getName().c_str(), dest->getType().getName().c_str());
        }
    }

    nbool me::onVisit(visitInfo i, defPropExpr& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: defPropExpr: check duplication");
        const nbicontainer& top = thread::get()._getNowFrame().subs();
        str eval = me.getEval();
        if(!eval)
            return posError(errCode::TYPE_NOT_EXIST, me, me.getName().c_str()), true;
        if(eval->isSub<nVoid>())
            return posError(errCode::VOID_CANT_DEFINED, me), true;

        NAMU_I("verify: defPropExpr: check whether make a void container.");
        const narr& beans = eval->getType().getBeans();
        for(const node& bean : beans)
            if(bean.isSub<nVoid>())
                return posError(errCode::NO_VOID_CONTAINER, me), true;

        const ntype& t = eval->getType();
        const nchar* typeName = nul(t) ? "null" : t.getName().c_str();
        if(nul(top)) return true;
        if(top.has(me.getName()))
            return posError(errCode::ALREADY_DEFINED_VAR, me, me.getName().c_str(), typeName), true;


        // 'check duplication' must be front of 'is %s definable':
        std::string name = me.getName();
        NAMU_I("verify: defPropExpr: is %s definable?", name.c_str());
        if(name == "") return posError(errCode::HAS_NO_NAME, me), true;
        const node& org = me.getOrigin();
        if(nul(org)) return posError(errCode::NO_ORIGIN, me, name.c_str()), true;

        if(nul(t))
            posError(errCode::CANT_DEF_VAR, me, name.c_str(), typeName);

        // when you define variable, I need to clone it:
        //  if don't, it may be incomplete object.
        if(!eval->canRun(args()))
            posError(errCode::DONT_HAVE_CTOR, me, name.c_str());
        else {
            node* new1 = new mockNode(*eval);
            if(me._where)
                me._where->add(name.c_str(), new1);
            else
                thread::get()._getNowFrame().addLocal(name, *new1);
        }
        return true;
    }

    nbool me::onVisit(visitInfo i, defSeqExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: defSeqExpr: check lhs & rhs");
        if(nul(me.getStart())) return posError(errCode::LHS_IS_NULL, me), true;
        if(nul(me.getEnd())) return posError(errCode::RHS_IS_NULL, me), true;

        NAMU_I("verify: defSeqExpr: lhs & rhs is sort of Int?");
        if(!me.getStart().isImpli<nInt>()) return posError(errCode::SEQ_SHOULD_INT_COMPATIBLE, me), true;
        if(!me.getEnd().isImpli<nInt>()) return posError(errCode::SEQ_SHOULD_INT_COMPATIBLE, me), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, defArrayExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: defArrayExpr: check all elements");
        const node& type = me.getArrayType();
        if(nul(type)) return posError(errCode::ELEM_TYPE_DEDUCED_NULL, me), true;
        if(type.isSuper<obj>())
            return posError(errCode::ELEM_TYPE_DEDUCED_WRONG, me, type.getType().getName().c_str()), true;
        if(type.isSub<nVoid>())
            return posError(errCode::ELEM_TYPE_NOT_VOID, me), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, FBOExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: FBOExpr: lhs & rhs should bind something.");
        const node& lhs = me.getLeft();
        const node& rhs = me.getRight();
        if(nul(lhs)) return posError(errCode::LHS_IS_NULL, me), true;
        if(nul(rhs)) return posError(errCode::RHS_IS_NULL, me), true;

        NAMU_I("verify: FBOExpr: finding eval of l(r)hs.");
        str lEval = lhs.getEval();
        str rEval = rhs.getEval();
        if(!lEval) return posError(errCode::LHS_IS_NULL, me), true;
        if(!rEval) return posError(errCode::RHS_IS_NULL, me), true;

        if(!checkEvalType(*lEval)) return posError(errCode::LHS_IS_NOT_ARITH, me, lEval->getType().getName().c_str()), true;
        if(!checkEvalType(*rEval)) return posError(errCode::RHS_IS_NOT_ARITH, me, rEval->getType().getName().c_str()), true;

        if(nul(lEval->deduce(*rEval)))
            return posError(errCode::IMPLICIT_CAST_NOT_AVAILABLE, me,
                    lEval->getType().getName().c_str(), rEval->getType().getName().c_str()), true;

        auto r = me.getRule();
        if((lEval->isSub<nStr>() || rEval->isSub<nStr>())) {
            switch(r) {
                case FBOExpr::AND: case FBOExpr::OR: case FBOExpr::SUB: case FBOExpr::DIV:
                case FBOExpr::MOD: case FBOExpr::BITWISE_AND: case FBOExpr::BITWISE_XOR:
                case FBOExpr::BITWISE_OR: case FBOExpr::LSHIFT: case FBOExpr::RSHIFT:
                    return posError(errCode::STRING_IS_NOT_PROPER_TO_OP, me), true;

                default:;
            }
        }
        return true;
    }

    nbool me::onVisit(visitInfo i, FUOExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: FUOExpr: string isn't proper to any FUO operator");
        str eval = me.getEval();
        if(eval && eval->isImpli<nStr>())
            return posError(errCode::STRING_IS_NOT_PROPER_TO_OP, me), true;

        return true;
    }

    nbool me::onVisit(visitInfo i, getExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        // TODO: I have to check that the evalType has what matched to given _params.
        // Until then, I rather use as() func and it makes slow emmersively.
        NAMU_I("verify: getExpr: isRunnable: %s.%s", me.getType().getName().c_str(), me.getSubName().c_str());
        if(!me.getEval()) return posError(errCode::WHAT_IS_THIS_IDENTIFIER, me, me.getSubName().c_str()), true;
        auto matches = me._get(true);
        if(matches.isEmpty()) {
            const node& from = me.getMe();
            return posError(errCode::CANT_ACCESS, me, me._name.c_str(), from.getType().getName().c_str()), true;
        }
        if(matches.len() >= 2) {
            // TODO: leave all ambigious candidates as err.
            return posError(errCode::AMBIGIOUS_ACCESS, me, i.name.c_str()), true;
        }
        str got = matches.getMatch();
        NAMU_I("verify: getExpr: isRunnable: got=%s, me=%s", got->getType().getName().c_str(),
                me.getType().getName().c_str());

        NAMU_I("verify: getExpr: accesses to incomplete 'me' object");
        str asedMe = me.getMe().as<node>();
        if(asedMe && !asedMe->isComplete())
            return posError(errCode::ACCESS_TO_INCOMPLETE, me), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, retExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: retExpr: checks evalType of func is matched to me");
        const baseFunc& f = thread::get().getNowFrame().getFunc();
        if(nul(f)) return posError(errCode::NO_FUNC_INFO, me), true;

        str myEval = me.getEval();
        if(!myEval) return posError(errCode::EXPR_EVAL_NULL, me), true;

        NAMU_I("verify: retExpr: myEval=%s", myEval->getType().getName().c_str());
        const ntype& myType = myEval->getType();
        str retType = f.getRet()->as<node>();
        const type& fType = retType->getType();

        NAMU_I("verify: retExpr: checks return[%s] == func[%s]", myType.getName().c_str(),
            fType.getName().c_str());

        if(!myType.isSub<err>() && !myType.isImpli(fType))
            return posError(errCode::RET_TYPE_NOT_MATCH, me, myType.getName().c_str(), fType.getName().c_str()), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, runExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: runExpr: is it possible to run?");
        if(nul(me.getMe())) return posError(errCode::DONT_KNOW_ME, me), true;

        str ased = me.getMe().getEval();
        if(!ased) return posError(errCode::DONT_KNOW_ME, me), true;

        node& anySub = me.getSubject();
        if(nul(anySub)) return posError(errCode::FUNC_NOT_EXIST, me), true;

        NAMU_I("verify: runExpr: anySub[%s]", anySub.getType().getName().c_str());

        args& a = me.getArgs();
        a.setMe(*ased);

        getExpr& cast = anySub.cast<getExpr>();
        if(!nul(cast))
            cast.setMe(*ased);

        str derivedSub = anySub.as<node>();
        if(!derivedSub) return posError(errCode::CANT_ACCESS, me, ased->getType().getName().c_str(), "sub-node"), true;

        NAMU_I("verify: runExpr: derivedSub[%s]", derivedSub->getType().getName().c_str());
        if(!derivedSub->canRun(me.getArgs())) {
            const baseFunc& derivedCast = derivedSub->cast<baseFunc>();
            std::string params = nul(derivedCast) ? "ctor" : _asStr(derivedCast.getParams());
            return posError(errCode::OBJ_WRONG_ARGS, me, i.name.c_str(), me.getArgs().asStr().c_str(), params.c_str()), true;
        }

        a.setMe(nulOf<baseObj>());
        return true;
    }

    std::string me::_asStr(const params& ps) {
        std::string ret;
        nbool first = true;
        for(const param& p : ps) {
            ret += (first ? "" : ",") + p.getOrigin().getType().getName();
            first = false;
        }

        return ret;
    }

    nbool me::onVisit(visitInfo i, func& me) {
        onVisit(i, (func::super&) me);

        obj& meObj = thread::get()._getNowFrame().getMe().cast<obj>(); // TODO: same to 'thread::get().getNowFrame().getMe().cast<obj>();'
        if(nul(meObj)) return posError(errCode::FUNC_REDIRECTED_OBJ, me), true;

        NAMU_I("verify: check func duplication");
        const nbicontainer& top = meObj.getShares();
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

            for(int n=0; n < castPs.len() ;n++) {
                str lhs = castPs[n].getOrigin().getEval();
                str rhs = me.getParams()[n].getOrigin().getEval();
                if(lhs->getType() != rhs->getType())
                    return false;
            }

            return true;
        });
        if(!nul(errFound))
            posError(errCode::ALREADY_DEFINED_FUNC, me, i.name.c_str());

        //  obj or property shouldn't have same name to any func.
        NAMU_I("verify: check func has same name to field");
        if(!nul(meObj.getOwns().getContainer().get(i.name)))
            posError(errCode::ALREADY_DEFINED_IDENTIFIER, me, i.name.c_str());

        NAMU_I("verify: func: main func return type should be int or void");
        if(i.name == starter::MAIN) {
            str ret = me.getRet();
            if(!ret->isSub<nInt>() && !ret->isSub<nVoid>())
                posError(errCode::MAIN_FUNC_RET_TYPE, me);
        }

        NAMU_I("verify: func: retType exists and stmts exist one at least");
        str retType = me.getRet();
        if(!retType) return posError(errCode::NO_RET_TYPE, me), true;
        if(!retType->isSub(ttype<node>::get()))
            return posError(errCode::WRONG_RET_TYPE, me, retType->getType().getName().c_str()), true;

        blockExpr& blk = (blockExpr&) me.getBlock();
        if(nul(blk) || blk.getStmts().len() <= 0) {
            if(i.name == starter::MAIN)
                posError(errCode::MAIN_SHOULD_HAVE_STMTS, blk);
            /*TODO: uncomment after implement abstract:
            if(!func.isAbstract() && !retType->isSub<nVoid>())
                error(...)
            */
            return true;
        }

        NAMU_I("verify: func: 'break' or 'next' can't be used to last stmt");
        const node& lastStmt = *blk.getStmts().last();
        if(lastStmt.isSub<retStateExpr>())
            return posError(errCode::FUNC_SHOULD_RETURN_SOMETHING, lastStmt), true;

        NAMU_I("verify: func[%s]: %s iterateBlock[%d]", i.name.c_str(), me.getType().getName().c_str(),
                me._blk->subs().len());

        // sequence of adding frame matters:
        //  object scope was added at 'onVisit(visitInfo, baseObj&)
        //  parameters of func is second:
        scope* s = new scope();
        for(const auto& p : me.getParams()) {
            if(p.getOrigin().isSub<nVoid>()) {
                posError(errCode::PARAM_NOT_VOID, me, p.getName().c_str());
                continue;
            }
            s->add(p.getName(), *(node*) p.getOrigin().as<node>()->clone());
        }

        //  function's subs are third:
        me.inFrame(*s);

        //  blockExpr is fourth:
        //      blockExpr shouldn't interact the frame on their side. that's concept.
        blk.inFrame();
        return true;
    }

    void me::_verifyMgdFuncImplicitReturn(func& me) {
        str ret = me.getRet()->as<node>();
        const type& retType = ret->getType();
        const node& lastStmt = *me.getBlock().getStmts().last();
        NAMU_I("verify: func: last stmt[%s] should matches to return type[%s]",
                lastStmt.getType().getName().c_str(), retType.getName().c_str());

        if(!lastStmt.isSub<retExpr>() && retType == ttype<nVoid>::get()) {
            NAMU_I("verify: func: implicit return won't verify when retType is void.");
            return;
        }

        str lastEval = lastStmt.getEval();
        if(!lastEval) return posError(NO_RET_TYPE, lastStmt);
        const ntype& lastType = lastEval->getType(); // to get type of expr, always uses evalType.
        if(nul(lastType)) return posError(NO_RET_TYPE, lastStmt);
        if(!lastType.isSub<err>() && !lastType.isImpli(retType)) return posError(errCode::RET_TYPE_NOT_MATCH, lastStmt, lastType.getName().c_str(),
                retType.getName().c_str());
    }

    void me::_prepare() {
        super::_prepare();
        _recentLoops.clear();

        baseObj& root = getTask().cast<baseObj>();
        if(!nul(root))
            // before verify obj and its subs, I need to register onto frame.
            root.inFrame();
    }

    void me::_postpare() {
        baseObj& root = getTask().cast<baseObj>();
        if(!nul(root))
            root.outFrame();
    }

    void me::onLeave(visitInfo i, func& me) {
        me.getBlock().outFrame();
        me.outFrame(scope());
    }

    nbool me::onVisit(visitInfo i, baseObj& me) {
        GUARD("verify: baseObj: me[%s]", me.getType().getName().c_str());

        me.inFrame();

        frame& fr = thread::get()._getNowFrame();
        NAMU_I("verify: baseObj: %s push me[%x] len=%d", fr.getMe().getType().getName().c_str(),
                &fr.getMe(), me.subs().len());

        NAMU_I("verify: baseObj: iterate all subs and checks void type variable");
        for(const node& elem : me.subs())
            if(elem.isSub<nVoid>())
                // don't finsh and return. i need to check all subs by calling super::onVisit().
                posError(errCode::VOID_CANT_DEFINED, elem);

        onVisit(i, (baseObj::super&) me);
        return true;
    }

    void me::onLeave(visitInfo i, baseObj& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());
        me.outFrame();
    }

    nbool me::onVisit(visitInfo i, genericObj& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: genericObj: cache check");
        for(auto e : me._cache)
            if(nul(e.second))
                posError(errCode::MAKE_GENERIC_FAIL, me, e.first.c_str());
        return true;
    }

    void me::onLeave(visitInfo i, genericObj& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: genericObj: onLeave");
        // DO NOTHING, BUT LEAVE THIS FUNC:
        //  if I don't have this func, getGenericExpr::super (=baseObj)'s one will be called.
        //  and me pointer will be erased too inside the func.
    }

    nbool me::onVisit(visitInfo i, forExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        str container = me._container;
        str conAsed = container->getEval();
        if(!conAsed)
            return posError(errCode::CONTAINER_IS_NULL, me), true;
        str elemType = conAsed->run("getElemType");
        if(!elemType)
            return posError(errCode::ELEM_TYPE_IS_NULL, me), true;

        const std::string& name = me.getLocalName();
        NAMU_I("verify: forExpr: define iterator '%s %s'", elemType->getType().getName().c_str(),
                name.c_str());

        me.getBlock().inFrame();
        thread::get()._getNowFrame().addLocal(name, *((node*) elemType->clone()));
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(visitInfo i, forExpr& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: forExpr: eval Value check: is an array?");
        tstr<arr> eval = me.getEval();
        if(!eval) return posError(errCode::LOOP_NO_RET_ARR, me);

        NAMU_I("verify: forExpr: onLeave");
        me.getBlock().outFrame();
        _recentLoops.pop_back();
    }

    nbool me::onVisit(visitInfo i, whileExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: whileExpr: onVisit");
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(visitInfo i, whileExpr& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: whileExpr: eval Value check: is an array?");
        tstr<arr> eval = me.getEval();
        if(!eval) return posError(errCode::LOOP_NO_RET_ARR, me);

        NAMU_I("verify: whileExpr: onLeave");
        _recentLoops.pop_back();
    }

    nbool me::onVisit(visitInfo i, breakExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: breakExpr: declared outside of loop?");
        if(_recentLoops.size() <= 0) return posError(errCode::BREAK_OUTSIDE_OF_LOOP, me), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, nextExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: nextExpr: declared outside of loop?");
        if(_recentLoops.size() <= 0) return posError(errCode::NEXT_OUTSIDE_OF_LOOP, me), true;
        return true;
    }

    nbool me::onVisit(visitInfo i, ifExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NAMU_I("verify: ifExpr: condition-expr can be casted into bool?");
        node& condition = me.getCondition();
        if(nul(condition))
            return posError(errCode::CONDITION_IS_EMPTY, me), true;
        if(!condition.is<nBool>())
            return posError(errCode::CONDITION_CANT_CAST_TO_BOOL, me), true;
        return true;
    }
}
