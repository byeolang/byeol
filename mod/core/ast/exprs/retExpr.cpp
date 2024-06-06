#include "retExpr.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "../../frame/thread.hpp"
#include "../../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(retExpr), DEF_VISIT())

    me::retExpr(const node& ret): _ret(ret) {}
    me::retExpr(): _ret(nVoid::singletone()) {}

    str me::run(const args& a) {
        if(!_ret) return str(nVoid::singletone());
        if(_ret->isSub<baseObj>()) return _ret; // case: obj

        str ret = _ret->as<node>(); // case: expr
        thread& thr = thread::get();
        auto& fr = thr._getNowFrame();
        str fRet = fr.getFunc().getRet();
        if(_isEx(*ret, *fRet)) {
            thr.getEx().add(ret->cast<err>());
        }
        fr.setRet(*ret);
        return ret;
    }

    nbool me::_isEx(const node& got, const node& funcRet) {
        return got.isSub<err>() && !funcRet.isSub<err>();
    }

    node& me::getRet() {
        return *_ret;
    }

    priorType me::prioritize(const args& a) const {
        if(_ret)
            return _ret->prioritize(a);
        return super::prioritize(a);
    }

    str me::getEval() const {
        if(!_ret)
            return _ret;

        return _ret->getEval();
    }
}
