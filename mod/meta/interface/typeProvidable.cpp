#include "typeProvidable.hpp"

#include "../type/ttype.hpp"

namespace nm {

    NM_DEF_ME(typeProvidable)

    nbool me::operator==(const me& rhs) const {
        if(nul(rhs)) return false;
        if(getType() != rhs.getType()) return false;

        return _onSame(rhs);
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    nbool me::isSub(const typeProvidable& it) const { return isSub(it.getType()); }

    nbool me::isSub(const type& it) const { return getType().isSub(it); }

    nbool me::isSuper(const type& it) const { return getType().isSuper(it); }

    void* me::cast(const type& to) {
        if(!getType().isSub(to)) return nullptr;

        return this;
    }

    nbool me::_onSame(const me& rhs) const { return this == &rhs; }
} // namespace nm
