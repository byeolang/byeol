#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/container/native/tnmap.inl"
#include "core/ast/node.hpp"
#include "core/ast/modifier/dumModifier.hpp"

namespace by {
    BY(DEF_ME(dumModifier))

    nbool me::isPublic() const { return true; }

    void me::setPublic(nbool newPublic) {}

    nbool me::isExplicitOverride() const { return false; }

    void me::setExplicitOverride(nbool isOverride) {}
}
