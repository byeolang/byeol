#include "core/ast/node.hpp"
#include "core/ast/modifier/modifier.hpp"

namespace nm {
    NM(DEF_ME(modifier))

    me::modifier(): modifier(true, false) {}

    me::modifier(nbool newPublic, nbool newExplicitOverride):
        _isPublic(newPublic), _isExplicitOverride(newExplicitOverride) {}

    nbool me::isPublic() const { return _isPublic; }

    void me::setPublic(nbool newPublic) { _isPublic = newPublic; }

    nbool me::isExplicitOverride() const { return _isExplicitOverride; }

    void me::setExplicitOverride(nbool isOverride) { _isExplicitOverride = isOverride; }
}
