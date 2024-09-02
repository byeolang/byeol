#include "expander.hpp"

#include "../../ast/exprs/getGenericExpr.hpp"
#include "../../ast/func.hpp"
#include "../../ast/node.inl"
#include "../../ast/obj.hpp"
#include "../../frame/frameInteract.hpp"
#include "../../frame/thread.hpp"
#include "../worker.inl"
#include "verifier.hpp"

namespace nm {

    NM(DEF_ME(expander))

    nbool me::expansion::isExpanded() const {
        if(!fun) return true;
        return fun->getBlock().getStmts().len() <= 0;
    }

#define GUARD(...) \
    if(isFlag(me::GUARD)) NM_I(__VA_ARGS__)

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
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        NM_DI("expand: obj: %s", i);

        _obj.bind(me);
        me.inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, obj& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        me.outFrame();
        _obj.rel();

        if(nul(me.sub(baseObj::EXPAND_NAME))) me.setState(PARSED);
    }

    nbool me::onVisit(const visitInfo& i, func& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        _func.bind(me);
        me.inFrame(); // don't need to inFrame for args.
                      // because what this want to do is just collect @expand funcs.
        NM_I("expand: func: %s", i);
        for(const auto& p: me.getParams())
            ((node&) p.getOrigin()).accept(i, *this);

        if(i.name == baseObj::EXPAND_NAME) {
            NM_I("expand: func: found expand");
            _stack[&_obj.get()] = {*_obj, me};
        }

        me.getBlock().inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, func& me) {
        GUARD("%s.onLeave(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        me.getBlock().outFrame();
        me.outFrame();
        _func.rel();
    }

    nbool me::onVisit(const visitInfo& i, getGenericExpr& me) {
        GUARD("%s.onVisit(%s)", getType().getName().c_str(), me.getType().getName().c_str());

        // this lets genericOrigin make a their generic obj.
        obj& generalizedOrg = me.getEval().cast<obj>();
        if(nul(generalizedOrg)) return true;

        obj& prevObj = *_obj;
        func& prevFunc = *_func;
        generalizedOrg.accept(i, *this);
        _func.bind(prevFunc);
        _obj.bind(prevObj);
        return true;
    }

    void me::_rel() { _stack.clear(); }

    void me::_expand() {
        GUARD(" ===================================");
        GUARD("          expandLoop");
        GUARD(" ===================================");
        errReport e;
        ncnt n = 0;
        while(_stack.size() > 0) {
            e.rel();
            GUARD("|--- %dth try: running %d expand track... ---|", ++n, _stack.size());
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

        _onEndErrReport(e);
        getReport().add(e);
        _stack.clear();
        GUARD(" ==== end of expandLoop ==== ");
    }

    nbool me::_expandAll(errReport& rpt) {
        GUARD("|--- expand: tryExpand: evaluation[%d] remains ---|", _stack.size());
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

                    GUARD("|--- expand: evalFunc(%x).len = %d ---|", &fun, stmts.len());

                    nbool isChanged = false;
                    for(int n = 0; n < stmts.len();) {
                        ncnt prevErrCnt = rpt.len();
                        verifier v;
                        v.setReport(rpt).setTask(stmts[n]).setFlag(0).work();
                        if(rpt.len() > prevErrCnt) {
                            // if there was an error, proceed next stmt.
                            // TODO: it uses len() for counting errors.
                            //       but one of them could be just warning.
                            GUARD("|--- expand: evalFunc(%x): eval failed on stmt[%d] ---|", &fun,
                                n);
                            n++;
                            continue;
                        }

                        stmts[n].run();

                        GUARD("|--- expand: evalFunc(%x): SUCCESS! stmt[%d] pre-evaluated.", &fun,
                            n);
                        stmts.del(n);
                        me.setState(PARSED);
                        isChanged = true;
                    } // end of inner

                    GUARD("|--- expand: end of evalFunc(%x) stmt[%d] left ---|", &fun, stmts.len());
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
