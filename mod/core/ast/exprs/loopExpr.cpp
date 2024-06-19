#include "loopExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../../frame/thread.hpp"
#include "breakExpr.hpp"
#include "nextExpr.hpp"
#include "../../builtin/container/mgd/arr.hpp"
#include "../../builtin/container/mgd/tdumArr.hpp"

namespace namu {

    NAMU(DEF_ME(loopExpr), DEF_VISIT())

    me::loopExpr(const blockExpr& blk): _blk(blk) {}

    blockExpr& me::getBlock() const {
        return *_blk;
    }

    str me::getEval() const {
        return _eval;
    }

    nbool me::setEval(const node& new1) const {
        return _eval.bind(new1);
    }

    nbool me::_postprocess() const {
        frame& fr = thread::get()._getNowFrame();
        const node& ret = fr.getRet();
        if(nul(ret)) return false;
        if(ret.isSub<nextRet>()) {
            fr.setRet();
            return false;
        }
        if(ret.isSub<breakRet>()) fr.setRet(); // after I go out of the loop, I should clear break state.

        // stop the loop. I found the return value of the func.
        return true;
    }

    arr& me::_preprocess() const {
        node& eval = *getEval();
        if(nul(eval))
            return NAMU_E("eval is null "), nulOf<arr>();
        static dumArr inner;
        return isReturnable() ? *new arr(eval.getType().getBeans()[0]) : inner;
    }

    nbool me::isReturnable() const { return _isReturnable; }
    void me::setReturnable(nbool isReturnable) { _isReturnable = isReturnable; }
}
