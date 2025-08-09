#include "core/ast/exprs/loopExpr.hpp"

#include "core/builtin/container/mgd/arr.hpp"
#include "core/builtin/container/mgd/tdumArr.hpp"
#include "core/frame/frameInteract.hpp"
#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/exprs/breakExpr.hpp"
#include "core/ast/exprs/nextExpr.hpp"
#include "core/ast/exprs/retExpr.hpp"

namespace by {

    BY(DEF_ME(loopExpr), DEF_VISIT())

    me::loop::loop(arr* ret): _ret(ret) {}

    arr* me::loop::getRet() { return _ret.get(); }

    void me::loop::run(blockExpr& blk, frame& fr) {
        str res = blk.run();
        if(_ret) _ret->add(*res);
    }

    nbool me::loop::postprocess(frame& fr) {
        const node& ret = fr.getRet() OR.ret(true);
        if(ret.isSub<nextRet>()) return fr.setRet(nullptr), true;
        if(ret.isSub<breakRet>())
            return fr.setRet(nullptr),
                   false; // after I go out of the loop, I should clear break state.
        // or stop the loop. I found the return value of the func.
        return false;
    }

    me::loopExpr(const blockExpr& blk): _blk(blk) {}

    blockExpr* me::getBlock() const { return _blk.get(); }

    str me::getEval() const {
        WHEN(_eval).ret(*_eval);

        return _eval = _makeEval();
    }

    str me::_makeEval() const {
        str res = getBlock() TO(getEval()) OR.ret(str());

        if(res->isSub<retExpr>()) return res;
        return new arr(*res->as<baseObj>());
    }

    str me::run(const args& a) {
        auto addr = platformAPI::toAddrId(this);
        blockExpr& blk = getBlock() OR.err("%s blk is null", addr).ret(str());
        tstr<loop> l = _makeLoop(_makeRet().get()) OR.err("%s loop is null", addr).ret(str());

        frame& fr = thread::get()._getNowFrame() OR.exErr(THERE_IS_NO_FRAMES_IN_THREAD).ret(str());
        while(l->isLooping()) {
            frameInteract f1(getBlock());
            l->run(blk, fr);
            if(!l->postprocess(fr)) break;
        }
        return l->getRet();
    }

    tstr<arr> me::_makeRet() const {
        static dumArr inner;
        str eval = getEval() OR.err("eval is null ").ret(nullptr);
        WHEN(!eval->isSub<arr>()).ret(nullptr);

        return *new arr(*eval->getType().getParams()[0].getOrigin().as<baseObj>());
    }
} // namespace by
