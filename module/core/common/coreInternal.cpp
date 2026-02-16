#include "coreInternal.hpp"
#include "core/ast/baseObj.hpp"
#include "core/type/mgdType.hpp"
#include "core/ast/modifier//modifier.hpp"

namespace by {
    BY(DEF_ME(coreInternal))

    void me::setSrc(node& me, const src& s) { me._setSrc(s); }

    void me::setType(baseObj& me, const mgdType& new1) { me._setType(new1); }

    void me::setModifier(baseObj& me, const modifier& mod) { me._setModifier(mod); }

    void me::setOrigin(baseObj& me, const baseObj& newOrg) { me._org.bind(newOrg); }
}
