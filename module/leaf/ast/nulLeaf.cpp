#include "leaf/ast/nulLeaf.hpp"

namespace by {

    NM_DEF_ME(nulLeaf)

    me::nulLeaf(): super() {}

    const std::string& me::asStr() const {
        static std::string inner = "";
        return inner;
    }

    nchar me::asChar() const { return '\0'; }

    nint me::asInt() const { return 0; }

    nbool me::asBool() const { return false; }

    const type& me::getType() const { return ttype<me>::get(); }

    nbool me::isExist() const { return false; }

    me& me::get() {
        static me inner;
        return inner;
    }
} // namespace by
