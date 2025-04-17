#include "expander.hpp"

#include "../../../ast/exprs/getGenericExpr.hpp"
#include "../../../ast/exprs/defAssignExpr.hpp"
#include "../../../ast/exprs/asExpr.hpp"
#include "../../../ast/func.hpp"
#include "../../../ast/node.inl"
#include "../../../ast/obj.hpp"
#include "../../../frame/frameInteract.hpp"
#include "../../../frame/thread.hpp"
#include "../../worker.inl"
#include "../verifier.hpp"
#include "convergence.hpp"

namespace nm {

    NM(DEF_ME(expander))

    namespace {
        static ncnt _stepN = 0;
    }

    nbool me::expansion::isExpanded() const {
        WHEN(!fun).ret(true);
        return fun->getBlock().getStmts().len() <= 0;
    }

#define _GUARD(msg)                                         \
    if(isFlag(GUARD)) do {                                  \
            NM_I("'%s' %s@%s: " msg, i, me.getType(), &me); \
            _stepN = 0;                                     \
    } while(0)

    me::expander() { rel(); }

    void me::rel() {
        _rel();

        super::rel();
    }

    void me::_onWork() {
        super::_onWork();
        _expand();
    }

    nbool me::onVisit(const visitInfo& i, defAssignExpr& me, nbool) {
        WHEN(!me.getExplicitType()).ret(true);
        _GUARD("defAssignExpr.onVisit()");

        tstr<convergence> req = new convergence(*_obj.back(), *_funcs.back(), [&]() -> nbool {
            str type = me.getExplicitType() TO(template as<node>()) OR_RET false;
            WHEN(type->isSub<expr>() || type->isSub<nVoid>()).ret(false);
            me.setExplicitType(*type);
            return true;
        });
        if(!req->convergeWithoutFrame()) _cons.add(*req);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, asExpr& me, nbool) {
        _GUARD("asExpr.onVisit()");

        tstr<convergence> req = new convergence(*_obj.back(), *_funcs.back(), [&]() -> nbool {
            str ased = me.getAs() TO(template as<baseObj>()) OR_RET false;
            WHEN(ased->isSub<expr>()).ret(false);
            me.setAs(*ased);
            return true;
        });
        if(!req->convergeWithoutFrame()) _cons.add(*req);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, obj& me, nbool) {
        _GUARD("obj.onVisit()");

        _obj.push_back(&me);
        me.inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, obj& me, nbool) {
        _GUARD("obj.onLeave()");

        me.outFrame();
        _obj.pop_back();

        if(nul(me.sub(baseObj::EXPAND_NAME))) me.setState(PARSED);
    }

    nbool me::onVisit(const visitInfo& i, func& me, nbool) {
        _GUARD("func.onVisit()");
        WHEN(!onVisit(i, (baseFunc&) me, false)).ret(false);

        if(i.name == baseObj::EXPAND_NAME) {
            obj *o = _obj.back() OR_RET NM_E("obj stack is empty."), true;
            NM_I("func: found expand[%s.%s]", *o, me);
            _stack[o] = {*o, me};
        }

        me.getBlock().inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, func& me, nbool) {
        _GUARD("func.onLeave()");
        me.getBlock().outFrame();

        onVisit(i, (baseFunc&) me, false);
    }

    nbool me::onVisit(const visitInfo& i, baseFunc& me, nbool) {
        // you may wonder why I declared parameter for 'baseFunc', not func:
        //  becauase you may think that it's not necessary for an author of the baseFunc to use
        //  getExpr instance as they can use define tbaseObjOrigin.
        //
        //  but please think about when genericOrigin comes to here.
        //  arr and seq uses genericOrigin, and it can't tbaseObjOrigin because it's beyond
        //  over static area, it's dynamic. only namu language user can define the type 'T'
        //  in managed space.
        //
        //  So, eventually there is still a chance when baseFuncs instance should hold getExpr
        //  for their retType or parameterType which requires typeConverence feature.
        _GUARD("onVisit()");

        _funcs.push_back(&me);
        me.inFrame(); // don't need to inFrame for args.
                      // because what this want to do is just collect @expand funcs.
        NM_I("expand: func: %s", i);
        for(auto& p: me.getParams())
            if(!_onVisitParams(me, p)) ((node&) p.getOrigin()).accept(i, *this);

        _onVisitFuncRet(me);
        return true;
    }

    void me::onLeave(const visitInfo& i, baseFunc& me, nbool) {
        _GUARD("onLeave(%s)");

        me.outFrame();
        _funcs.pop_back();
    }

    nbool me::onVisit(const visitInfo& i, getGenericExpr& me, nbool) {
        _GUARD("onVisit(%s)");

        // this lets genericOrigin make a their generic obj.
        obj& generalizedOrg = me.getEval().cast<obj>() OR_RET true;
        generalizedOrg.accept(i, *this);
        return true;
    }

    nbool me::_onVisitParams(baseFunc& f, param& p) {
        node& org = (node&) p.getOrigin();
        [[maybe_unused]] const getExpr& isGet = org.cast<getExpr>() OR_RET false;

        // need to converge type:
        //  try once now. if it's not successful, it may refered symbol not expanded yet.
        //  so in that case, I put it to a queue to process after expansion.
        NM_I("converge type request for param[%s] of %s", p, f);
        tstr<baseConvergence> req = new paramConvergence(*_obj.back(), f, p, org);
        if(!req->convergeWithoutFrame()) // I'll converge it later.
            _cons.add(*req);
        return true;
    }

    void me::_onVisitFuncRet(baseFunc& f) {
        ntype& type = (ntype&) f.getType() OR_RET;
        const getExpr& ret = type.getRet().cast<getExpr>() OR_RET;

        // need to converge return type:
        NM_I("converge type request for ret[%s] of %s", ret.getName(), f);
        tstr<baseConvergence> req = new retConvergence(*_obj.back(), f, ret);
        if(!req->convergeWithoutFrame()) // I'll converge it later
            _cons.add(*req);
    }

    void me::_convergeTypes(errReport& rpt) {
        WHEN(rpt).err("type convergence cancelled. there is error during expand()").ret();

        for(auto& req: _cons)
            req.converge(); // only try once. and don't emit any errors when it fails.
                            // verifier will catch it.
    }

    void me::_rel() { _stack.clear(); }

    void me::_expand() {
        NM_I(" ===================================");
        NM_I("          expandLoop");
        NM_I(" ===================================");
        errReport e(false);
        ncnt n = 0;
        while(_stack.size() > 0) {
            e.rel();
            NM_I("|--- %dth try: running %d expansions... ---|", ++n, _stack.size());
            if(!_expandAll(
                   e)) { // this func actually remove elements of _stack if the func consumes it.
                // ok. there is no change after running one loop, which means, I think that
                // expander just found circular dependencies.
                NM_E("* * *");
                NM_E("I couldn't finish expansion. may be because of circular dependency.");
                NM_E("total %d expanding remains.", _stack.size());
                NM_E("errors:");
                e.dump();
                NM_E("* * *");
                break;
            }
        }

        _convergeTypes(e);
        _onEndErrReport(e);
        getReport().add(e);
        _stack.clear();
        NM_I(" ==== end of expandLoop ==== ");
    }

    nbool me::_expandAll(errReport& rpt) {
        nbool isChanged = false;
        for(auto e = _stack.begin(); e != _stack.end();) {
            auto& eval = e->second;
            if(_expand(rpt, eval)) {
                isChanged = true;
                if(eval.isExpanded()) {
                    _delEval(e++);
                    continue;
                }
            }
            ++e;
        }
        return isChanged;
    }

    nbool me::_expand(errReport& rpt, expansion& eval) {
        obj& me = *eval.me;
        frameInteract f1(me);

        func& fun = *eval.fun;
        frameInteract f2(fun);

        blockExpr& blk = fun.getBlock();
        frameInteract f3(blk);

        narr& stmts = blk.getStmts();
        NM_I("|--- %s.%s has %d stmts ---|", me, fun, stmts.len());

        nbool isChanged = false;
        for(int n = 0; n < stmts.len();) {
            ncnt prevErrCnt = rpt.len();
            verifier v;
            v.setReport(rpt).setTask(stmts[n]).setFlag(0).work();
            if(rpt.len() > prevErrCnt) {
                // if there was an error, proceed next stmt.
                // TODO: it uses len() for counting errors.
                //       but one of them could be just warning.
                NM_I("|--- %s.%s failed on %dth stmt ---|", me, fun, n);
                n++;
                continue;
            }

            stmts[n].run();

            NM_I("|--- SUCCESS! stmt[%d] pre-evaluated.", n);
            stmts.del(n);
            me.setState(PARSED);
            isChanged = true;
        } // end of inner

        NM_I("|--- end of %s.%s %d stmts left ---|", me, fun, stmts.len());
        return isChanged;
    }

    void me::_delEval(const std::map<obj*, expansion>::iterator& e) {
        e->second.me->subs().del(baseObj::EXPAND_NAME);
        _stack.erase(e);
    }
} // namespace nm
