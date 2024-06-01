#include "dumScope.hpp"

namespace namu {
    
    NAMU(DEF_ME(dumScope))

    me::dumScope() {}
    me::dumScope(const node& owner): super(owner) {}
    me::dumScope(const super& rhs): super(rhs) {} // initialization is allowed.

    nbool me::add(const std::string& key, const node& val) {
        return false;
    }

    me* me::cloneDeep() const {
        return (me*) super::cloneDeep();
    }
}