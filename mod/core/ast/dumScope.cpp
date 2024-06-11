#include "dumScope.hpp"
#include "node.hpp"

namespace namu {
    
    NAMU(DEF_ME(dumScope))

    me::dumScope() {}
    me::dumScope(const scope::super& org): super(org) {}

    nbool me::add(const std::string& key, const node& val) {
        return false;
    }
}
