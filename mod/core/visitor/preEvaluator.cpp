#include "preEvaluator.hpp"
#include "../ast/node.inl"
#include "../ast/obj.hpp"
#include "../ast/func.hpp"
#include "../frame/frameInteract.hpp"
#include "verifier.hpp"
#include "../ast/exprs/getGenericExpr.hpp"
#include "../frame/thread.hpp"
#include "../loader/worker/worker.inl"

namespace nm {

    NM(DEF_ME(preEvaluator))

    nbool me::evaluation::isEvaluated() const {
        if(!fun) return true;
        return fun->getBlock().getStmts().len() <= 0;
    }

#define GUARD(...) if(isFlag(me::GUARD)) NM_I(__VA_ARGS__)


    me::preEvaluator() {
        rel();
    }

    void me::rel() {
        _rel();

        super::rel();
    }

    void me::_onWork() {
        super::_onWork();
        _preEval();
    }

    nbool me::onVisit(visitInfo i, obj& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NM_DI("preEval: obj: %s", i);

        _obj.bind(me);
        me.inFrame();
        return true;
    }

    void me::onLeave(visitInfo i, obj& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        me.outFrame();
        _obj.rel();
    }

    nbool me::onVisit(visitInfo i, func& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        _func.bind(me);
        me.inFrame(); // don't need to inFrame for args.
                      // because what this want to do is just collect @preCtor funcs.
        NM_I("preEval: func: %s", i);
        for(const auto& p : me.getParams())
            ((node&) p.getOrigin()).accept(i, *this);

        if(i.name == baseObj::PRECTOR_NAME) {
            NM_I("preEval: func: found prector");
            _stack[&_obj.get()] = {*_obj, me};
        }

        me.getBlock().inFrame();
        return true;
    }

    void me::onLeave(visitInfo i, func& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        me.getBlock().outFrame();
        me.outFrame();
        _func.rel();
    }

    nbool me::onVisit(visitInfo i, getGenericExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        // this lets genericObj make a their generic obj.
        obj& genericObj = me.getEval().cast<obj>();
        if(nul(genericObj)) return true;

        obj& prevObj = *_obj;
        func& prevFunc = *_func;
        genericObj.accept(i, *this);
        _func.bind(prevFunc);
        _obj.bind(prevObj);
        return true;
    }

    void me::_rel() {
        _stack.clear();
    }

    void me::_preEval() {
        GUARD(" ===================================");
        GUARD("          preEvaluationLoop         ");
        GUARD(" ===================================");
        errReport e;
        ncnt n = 0;
        while(_stack.size() > 0) {
            e.rel();
            GUARD("|--- %dth try: running %d pre evaluation track... ---|", ++n, _stack.size());
            if(!_tryPreEvals(e)) { // this func actually remove elements of _stack if the func consumes it.
                // ok. there is no change after running one loop, which means, I think that
                // preEvaluator just found circular dependencies.
                NM_E("* * *");
                NM_E("I couldn't finish pre-evaluation. may be because of circular dependency.");
                NM_E("total %d pre-evaluations remains.", _stack.size());
                NM_E("errors:");
                e.dump();
                NM_E("* * *");
                break;
            }
        }

        _onEndErrReport(e);
        getReport().add(e);
        _stack.clear();
        GUARD(" ==== end of preEvaluationLoop ==== ");
    }

    nbool me::_tryPreEvals(errReport& rpt) {
        GUARD("|--- preEval: tryPreEvals: evaluation[%d] remains ---|", _stack.size());
        nbool isChanged = false;
        for(auto e=_stack.begin(); e != _stack.end() ;) {
            auto& eval = e->second;
            if(_tryPreEval(rpt, eval)) {
                isChanged = true;
                if(eval.isEvaluated()) {
                    _delEval(e++);
                    continue;
                }
            }
            ++e;
        }
        return isChanged;
    }

    nbool me::_tryPreEval(errReport& rpt, evaluation& eval) {
        obj& me = *eval.me;
        frameInteract f1(me); {
            func& fun = *eval.fun;
            frameInteract f2(fun); {
                blockExpr& blk = fun.getBlock();
                frameInteract f3(blk); {
                    narr& stmts = blk.getStmts();

                    GUARD("|--- preEval: evalFunc(%x).len = %d ---|", &fun, stmts.len());

                    nbool isChanged = false;
                    for(int n=0; n < stmts.len() ;) {
                        ncnt prevErrCnt = rpt.len();
                        verifier v;
                        v.setReport(rpt).setTask(stmts[n]).setFlag(0).work();
                        if(rpt.len() > prevErrCnt) {
                            // if there was an error, proceed next stmt.
                            // TODO: it uses len() for counting errors.
                            //       but one of them could be just warning.
                            GUARD("|--- preEval: evalFunc(%x): eval failed on stmt[%d] ---|", &fun, n);
                            n++;
                            continue;
                        }

                        stmts[n].run();

                        GUARD("|--- preEval: evalFunc(%x): SUCCESS! stmt[%d] pre-evaluated.", &fun, n);
                        stmts.del(n);
                        isChanged = true;
                    } // end of inner

                    GUARD("|--- preEval: end of evalFunc(%x) stmt[%d] left ---|", &fun, stmts.len());
                    return isChanged;
                }
            }
        }
    }

    void me::_delEval(std::map<obj*, evaluation>::iterator e) {
        e->second.me->subs().del(baseObj::PRECTOR_NAME);
        _stack.erase(e);
    }
}
