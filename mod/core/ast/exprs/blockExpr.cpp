#include "../../builtin/container/mgd/arr.hpp"
#include "blockExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "../../visitor/visitor.hpp"
#include "retStateExpr.hpp"

namespace namu {

    NAMU(DEF_ME(blockExpr), DEF_VISIT())

    me::blockExpr() {}

    narr& me::getStmts() {
        return _exprs;
    }

    clonable* me::cloneDeep() const {
        NAMU_DI("%s.cloneDeep()", getType().getName().c_str());

        me* ret = (me*) clone();
        ret->_exprs.rel();
        for(auto e=_exprs.begin(); e ;e++)
            ret->_exprs.add((node*) e->cloneDeep());

        return ret;
    }

    void me::inFrame(const bicontainable& args) {
        NAMU_DI("%s._onInFrame() %d stmts. frames.len[%d]", getType().getName().c_str(), getStmts()
                .len(), thread::get().getFrames().len());

        frame& fr = namu::thread::get()._getNowFrame();
        if(nul(fr)) {
            NAMU_E("fr == null");
            return;
        }

        fr.add(*new scope());
    }

    void me::outFrame(const bicontainable& args) {
        NAMU_DI("%s._onOutFrame() frames.len[%d]", getType().getName().c_str(), thread::get()
                .getFrames().len());

        frame& fr = namu::thread::get()._getNowFrame();
        if(nul(fr)) {
            NAMU_E("fr == null");
            return;
        }

        fr.del();
    }

    str me::run(const args& a) {
        // for optimization, blockExpr's frame should be controlled from outside of this this.
        // that's why I didn't use frameInteracter here. see forExpr::run().
        str ret;

        thread& th = thread::get();
        const auto& ex = th.getEx();
        const frame& fr = th.getNowFrame();
        nidx exN = ex.len() - 1; // blockExpr will judge exception occurs when exN is changed to after running one of its stmt.
        NAMU_DI("blockExpr: loop %d stmts", _exprs.len());
        for(auto& e : _exprs) {
            ret = e.as<node>(); // if e is expr, it runs(). if not, it returns itself.
            if(ex.len() > (exN + 1)) {
                tstr<err> last = *ex.last();
                NAMU_DI("err%d: '%s' exception found in block.\n", last->code, last->msg.c_str());
                return last; // return last err instance I got.
                             // so it's not the return type of what the func told, but it's okay.
                             // all derived err object can be assigned to any type.
            }
            if(!nul(fr.getRet())) break;
        }
        return ret;
    }

    str me::getEval() const {
        if(_exprs.isEmpty()) return str();
        return _exprs.last()->getEval();
    }
}
