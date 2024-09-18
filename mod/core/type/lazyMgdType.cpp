#include "lazyMgdType.hpp"

namespace nm {

    NM(DEF_ME(lazyMgdType))

    me::lazyMgdType(const std::string& name, const type& s, const params& ps,
        std::function<const node&()> retLazy):
        super(name, s, ps), _retLazy(retLazy) {}

    const node& me::getRet() const {
        const node& ret = super::getRet();
        if(!nul(ret)) return ret;

        me& unconst = (me&) *this;
        unconst.setRet(unconst._retLazy());
        return super::getRet();
    }
}
