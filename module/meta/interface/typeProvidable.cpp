#include "meta/interface/typeProvidable.hpp"

#include "meta/type/ttype.hpp"

namespace by {

    NM_DEF_ME(typeProvidable)

    nbool me::operator==(const me& rhs) const {
        WHEN(getType() != rhs.getType()).ret(false);

        return _onSame(rhs);
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    nbool me::isSub(const me& it) const { return isSub(it.getType()); }

    nbool me::isSub(const type& it) const { return getType().isSub(it); }

    nbool me::isSuper(const type& it) const { return getType().isSuper(it); }

    nbool me::isSuper(const me& it) const { return isSuper(it.getType()); }

    void* me::cast(const type& to) {
        WHEN(!getType().isSub(to)).ret(nullptr);
        return this;
    }

    nbool me::_onSame(const me& rhs) const { return this == &rhs; }
} // namespace by
