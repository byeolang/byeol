#include "retExpr.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "../../frame/thread.hpp"
#include "../../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(retExpr), DEF_VISIT())

    me::retExpr(const node& ret): _ret(ret) {}
    me::retExpr(): _ret(nVoid::singletone()) {}

    namespace {
        tstr<err> _returnEx(tstr<err> e) {
            thread& thr = thread::get();
            thr.getEx().add(*e);
            thr.getNowFrame().setRet(*e);
            return e;
        }
    }

    str me::run(const args& a) {
        if(!_ret) return str(nVoid::singletone());
        if(_ret->isSub<baseObj>()) return _ret; // case: obj

        auto& fr = thread::get().getNowFrame();

        str ret = _ret->as<node>(); // # check retValue is null or not.
        if(!ret) // ret should be void if there is no value to return. so 'null' not allowed here.
            return _returnEx(err::newErr(errCode::RETURN_VALUE_IS_NULL, getSrc().getName().c_str()));

        str fRet = fr.getFunc().getRet(); // # check exception occured during running func.
        if(_isEx(*ret, *fRet))
            return _returnEx(ret->cast<err>());

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
