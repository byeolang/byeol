#include "defaultCtor.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(defaultCtor), DEF_VISIT())

    me::defaultCtor(const node& org): super(org) {}

    str me::run(const args& a) {
        return str();
    }
}
