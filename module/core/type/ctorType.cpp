#include "ctorType.hpp"

namespace by {

    BY(DEF_ME(ctorType))

    me::ctorType(const type& superType, const params& ps, const node* ret): super(CTOR_NAME, superType, ps, false, ret) {}

    const std::string& me::getName() const { return CTOR_NAME; }
}
