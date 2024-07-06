#include "../../builtin/container/mgd/arr.hpp"
#include "blockExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "../../visitor/visitor.hpp"
#include "retStateExpr.hpp"

namespace nm {

    NM(DEF_ME(blockExpr), DEF_VISIT())

    me::blockExpr() {}

    narr& me::getStmts() {
        return _exprs;
    }

    clonable* me::cloneDeep() const {
        NM_DI("%s.cloneDeep()", *this);

        me* ret = (me*) clone();
        ret->_exprs.rel();
        for(auto e=_exprs.begin(); e ;e++)
            ret->_exprs.add((node*) e->cloneDeep());

        return ret;
    }

    void me::inFrame(const bicontainable& args) {
        NM_DI("%s._onInFrame() %s stmts. frames.len[%s]", *this, getStmts()
                .len(), thread::get().getFrames().len());

        frame& fr = nm::thread::get()._getNowFrame();
        if(nul(fr)) {
            NM_E("fr == null");
            return;
        }

        fr.add(*new scope());
    }

    void me::outFrame(const bicontainable& args) {
        NM_DI("%s._onOutFrame() frames.len[%s]", *this, thread::get()
                .getFrames().len());

        frame& fr = nm::thread::get()._getNowFrame();
        if(nul(fr)) {
            NM_E("fr == null");
            return;
        }

        fr.del();
    }

    str me::run(const args& a) {
        // for optimization, blockExpr's frame should be controlled from outside of this this.
        // that's why I didn't use frameInteracter here. see forExpr::run().
        str ret;
        auto addr = platformAPI::toAddrId(this);
        thread& th = thread::get();
        const auto& ex = th.getEx();
        const frame& fr = th.getNowFrame();
        nidx exN = ex.len() - 1; // blockExpr will judge exception occurs when exN is changed to after running one of its stmt.
        NM_DI("%s blockExpr: loop %s stmts", addr, _exprs.len());
        for(auto& e : _exprs) {
            ret = e.as<node>(); // if e is expr, it runs(). if not, it returns itself.
            if(ex.len() > (exN + 1)) {
                tstr<err> last = *ex.last();
                NM_DI("%s err%s(%s): '%s' exception found in block.\n", addr, last->code, err::getErrName(last->code), last->msg);
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
