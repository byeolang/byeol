#include "loopExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../../frame/thread.hpp"
#include "../../frame/frameInteract.hpp"
#include "breakExpr.hpp"
#include "nextExpr.hpp"
#include "retExpr.hpp"
#include "../../builtin/container/mgd/arr.hpp"
#include "../../builtin/container/mgd/tdumArr.hpp"

namespace namu {

    NAMU(DEF_ME(loopExpr), DEF_VISIT())

    me::loop::loop(arr& ret): _ret(ret) {}

    arr& me::loop::getRet() { return *_ret; }

    void me::loop::run(blockExpr& blk, frame& fr) {
        _ret->add(*blk.run());
    }

    nbool me::loop::postprocess(frame& fr) {
        const node& ret = fr.getRet();
        if(nul(ret)) return true;

        if(ret.isSub<nextRet>()) return fr.setRet(), true;
        if(ret.isSub<breakRet>()) return fr.setRet(), false; // after I go out of the loop, I should clear break state.
        // or stop the loop. I found the return value of the func.
        return false;
    }

    me::loopExpr(const blockExpr& blk): _blk(blk) {}

    blockExpr& me::getBlock() const {
        return *_blk;
    }

    str me::getEval() const {
        if(_eval) return *_eval;

        return _eval = _makeEval();
    }

    str me::_makeEval() const {
        str res = getBlock().getEval();
        if(!res) return str();

        return new arr(*res);
    }

    str me::run(const args& a) {
        blockExpr& blk = getBlock();
        if(nul(blk)) return NAMU_E("blk is null"), str();

        auto l = _makeLoop(*_makeRet());
        if(!l) return NAMU_E("loop is null"), str();

        frame& fr = thread::get()._getNowFrame();
        while(l->isLooping()) {
            frameInteract f1(getBlock()); {
                l->run(blk, fr);
                if(!l->postprocess(fr))
                    break;
            }
        }
        return l->getRet();
    }

    tstr<arr> me::_makeRet() const {
        static dumArr inner;
        node& eval = *getEval();
        if(nul(eval))
            return NAMU_E("eval is null "), nulOf<arr>();
        return *new arr(eval.getType().getBeans()[0]); // it's possible that this new arr contains
                                                       // 'retExpr' as type parameter
    }
}
