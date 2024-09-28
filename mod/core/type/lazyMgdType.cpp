#include "lazyMgdType.hpp"

namespace nm {

    NM(DEF_ME(lazyMgdType))

    me::lazyMgdType(const std::string& name, const type& s, paramLambda psLazy, retLambda retLazy):
        super(name, s, params()), _psLazy(psLazy), _retLazy(retLazy) {}

    params& me::getParams() {
        params& ps = super::getParams();
        if(!nul(ps)) return ps;

        _psLazy(ps);
        return ps;
    }

    const node& me::getRet() const {
        const node& ret = super::getRet();
        if(!nul(ret)) return ret;

        me& unconst = (me&) *this;
        unconst.setRet(unconst._retLazy());
        return super::getRet();
    }
}
