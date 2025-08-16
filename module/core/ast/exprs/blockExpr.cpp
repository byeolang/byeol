#include "core/ast/exprs/blockExpr.hpp"

#include "core/builtin/container/mgd/arr.hpp"
#include "core/builtin/primitive/nVoid.hpp"
#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/exprs/retStateExpr.hpp"
#include "core/builtin/err/nerr.hpp"

namespace by {

    BY(DEF_ME(blockExpr), DEF_VISIT())

    me::blockExpr() {}

    narr& me::getStmts() { return _exprs; }

    void me::rel() {
        _exprs.rel();
        _eval.rel();
    }

    void me::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        _exprs.onCloneDeep(rhs._exprs);
    }

    nbool me::isAbstract() const { return _exprs.isEmpty() && _eval; }

    void me::inFrame(const bicontainable*) const {
        BY_DI("%s._onInFrame() %d stmts. frames.len[%d]", *this, getStmts().len(), thread::get().getFrames().len());

        frame& fr = by::thread::get()._getNowFrame() OR.err("fr == null").ret();
        fr.add(*new scope());
    }

    void me::outFrame() const {
        BY_DI("%s._onOutFrame() frames.len[%d]", *this, thread::get().getFrames().len());

        frame& fr = by::thread::get()._getNowFrame() OR.err("fr == null").ret();
        fr.del();
    }

    str me::run(const args& a) {
        // for optimization, blockExpr's frame should be controlled from outside of this this.
        // that's why I didn't use frameInteracter here. see forExpr::run().
        str ret;
        auto addr = platformAPI::toAddrId(this);
        thread& th = thread::get();
        const auto& ex = th.getEx();
        const frame& fr = th.getNowFrame() OR.exErr(THERE_IS_NO_FRAMES_IN_THREAD).ret(str());
        nidx exN = ex.len() - 1; // blockExpr will judge exception occurs when exN is changed to
                                 // after running one of its stmt.
        BY_DI("%s blockExpr: loop %d stmts", addr, _exprs.len());
        for(auto& e: _exprs) {
            ret = e.as<node>(); // if e is expr, it runs(). if not, it returns itself.
            if(ex.len() > (exN + 1)) {
                tstr<baseErr> last = *ex.last();
                BY_DI("%s '%s' exception found in block.\n", addr, last->getMsg());
                return last; // return last err instance I got.
                             // so it's not the return type of what the func told, but it's okay.
                             // all derived err object can be assigned to any type.
            }
            if(fr.getRet()) break;
        }
        return ret;
    }

    str me::getEval() const {
        WHEN(_eval) .ret(_eval);
        WHEN(_exprs.isEmpty()) .ret(nVoid::singleton());

        return _exprs.last()->getEval();
    }

    void me::setEval(const node& newEval) { _eval.bind(newEval); }
} // namespace by
