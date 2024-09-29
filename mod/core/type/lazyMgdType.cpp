#include "lazyMgdType.hpp"

namespace nm {

    NM(DEF_ME(lazyMgdType))

    me::lazyMgdType(const std::string& name, const type& s, paramLambda psLazy, retLambda retLazy):
        super(name, s, params()), _psLazy(psLazy), _retLazy(retLazy) {}

    params& me::getParams() {
        params& ps = super::getParams();
        if(_psLazy) {
            _psLazy(ps);
            _psLazy = nullptr;
        }

        return ps;
    }

    const node& me::getRet() const {
        if(_retLazy) {
            me& unconst = (me&) *this;
            unconst.setRet(unconst._retLazy());
            _retLazy = nullptr;
        }
        return super::getRet();
    }
}
