#include "dumScope.hpp"
#include "node.hpp"

namespace nm {
    
    NM(DEF_ME(dumScope))

    me::dumScope() {}
    me::dumScope(const scope::super& org): super(org) {}

    nbool me::add(const std::string& key, const node& val) {
        return false;
    }

    me& me::singletone() {
        static me inner;
        return inner;
    }
}
