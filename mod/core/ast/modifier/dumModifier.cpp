#include "dumModifier.hpp"

namespace nm {
    NM(DEF_ME(dumModifier))

    nbool me::isPublic() const { return true; }
    void me::setPublic(nbool newPublic) {}

    nbool me::isExplicitOverride() const { return false; }
    void me::setExplicitOverride(nbool isOverride) {}
}
