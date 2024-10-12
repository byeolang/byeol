#include "expander.hpp"

#include "../../../ast/exprs/getGenericExpr.hpp"
#include "../../../ast/func.hpp"
#include "../../../ast/node.inl"
#include "../../../ast/obj.hpp"
#include "../../../frame/frameInteract.hpp"
#include "../../../frame/thread.hpp"
#include "../../worker.inl"
#include "../verifier.hpp"
#include "paramConvergence.hpp"
#include "retConvergence.hpp"

namespace nm {

    NM(DEF_ME(expander))

    namespace {
        static ncnt _stepN = 0;
    }

    nbool me::expansion::isExpanded() const {
        if(!fun) return true;
        return fun->getBlock().getStmts().len() <= 0;
    }

#define _GUARD(msg)                                                                \
    if(isFlag(GUARD)) do {                                                         \
            NM_I("'%s' %s@%s: " msg, i, me.getType(), platformAPI::toAddrId(&me)); \
            _stepN = 0;                                                            \
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

    nbool me::onVisit(const visitInfo& i, obj& me) {
        _GUARD("obj.onVisit()");

        _obj.push_back(&me);
        me.inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, obj& me) {
        _GUARD("obj.onLeave()");

        me.outFrame();
        _obj.pop_back();

        if(nul(me.sub(baseObj::EXPAND_NAME))) me.setState(PARSED);
    }

    nbool me::onVisit(const visitInfo& i, func& me) {
        _GUARD("func.onVisit()");
        if(!onVisit(i, (baseFunc&) me)) return false;

        if(i.name == baseObj::EXPAND_NAME) {
            obj* o = _obj.back() orRet NM_E("obj stack is empty."), true;
            NM_I("func: found expand[%s.%s]", *o, me);
            _stack[o] = {*o, me};
        }

        me.getBlock().inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, func& me) {
        _GUARD("func.onLeave()");
        me.getBlock().outFrame();

        onVisit(i, (baseFunc&) me);
    }

    nbool me::onVisit(const visitInfo& i, baseFunc& me) {
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

    void me::onLeave(const visitInfo& i, baseFunc& me) {
        _GUARD("onLeave(%s)");

        me.outFrame();
        _funcs.pop_back();
    }

    nbool me::onVisit(const visitInfo& i, getGenericExpr& me) {
        _GUARD("onVisit(%s)");

        // this lets genericOrigin make a their generic obj.
        obj& generalizedOrg = me.getEval().cast<obj>() orRet true;
        generalizedOrg.accept(i, *this);
        return true;
    }

    nbool me::_onVisitParams(baseFunc& f, param& p) {
        node& org = (node&) p.getOrigin();
        [[maybe_unused]] const getExpr& isGet = org.cast<getExpr>() orRet false;

        // need to converge type:
        //  try once now. if it's not successful, it may refered symbol not expanded yet.
        //  so in that case, I put it to a queue to process after expansion.
        NM_I("converge type request for param[%s] of %s", p, f);
        tstr<convergence> req = new paramConvergence(f, p, org);
        if(!req->converge()) // I'll converge it later.
            _cons.add(*req);
        return true;
    }

    void me::_onVisitFuncRet(baseFunc& f) {
        ntype& type = (ntype&) f.getType() orRet;
        const getExpr& ret = type.getRet().cast<getExpr>() orRet;

        // need to converge return type:
        NM_I("converge type request for ret[%s] of %s", ret.getName(), f);
        tstr<convergence> req = new retConvergence(f, ret);
        if(!req->converge()) // I'll converge it later
            _cons.add(*req);
    }

    void me::_convergeTypes(errReport& rpt) {
        if(rpt) return NM_E("type convergence cancelled. there is error during expand()"), void();

        for(auto& req: _cons)
            req.converge(); // only try once. and don't emit any errors when it fails.
                            // verifier will catch it.
    }

    void me::_rel() { _stack.clear(); }

    void me::_expand() {
        NM_I(" ===================================");
        NM_I("          expandLoop");
        NM_I(" ===================================");
        errReport e;
        ncnt n = 0;
        while(_stack.size() > 0) {
            e.rel();
            NM_I("|--- %dth try: running %d expand track... ---|", ++n, _stack.size());
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
        NM_I("|--- expand: tryExpand: evaluation[%d] remains ---|", _stack.size());
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
        {
            func& fun = *eval.fun;
            frameInteract f2(fun);
            {
                blockExpr& blk = fun.getBlock();
                frameInteract f3(blk);
                {
                    narr& stmts = blk.getStmts();

                    NM_I("|--- expand: evalFunc(%x).len = %d ---|", &fun, stmts.len());

                    nbool isChanged = false;
                    for(int n = 0; n < stmts.len();) {
                        ncnt prevErrCnt = rpt.len();
                        verifier v;
                        v.setReport(rpt).setTask(stmts[n]).setFlag(0).work();
                        if(rpt.len() > prevErrCnt) {
                            // if there was an error, proceed next stmt.
                            // TODO: it uses len() for counting errors.
                            //       but one of them could be just warning.
                            NM_I("|--- expand: evalFunc(%x): eval failed on stmt[%d] ---|", &fun,
                                n);
                            n++;
                            continue;
                        }

                        stmts[n].run();

                        NM_I("|--- expand: evalFunc(%x): SUCCESS! stmt[%d] pre-evaluated.", &fun,
                            n);
                        stmts.del(n);
                        me.setState(PARSED);
                        isChanged = true;
                    } // end of inner

                    NM_I("|--- expand: end of evalFunc(%x) stmt[%d] left ---|", &fun, stmts.len());
                    return isChanged;
                }
            }
        }
    }

    void me::_delEval(const std::map<obj*, expansion>::iterator& e) {
        e->second.me->subs().del(baseObj::EXPAND_NAME);
        _stack.erase(e);
    }
} // namespace nm
