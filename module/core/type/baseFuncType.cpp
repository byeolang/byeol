#include "core/type/baseFuncType.hpp"
#include "core/type/as/ases.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/type/as/baseFuncAses.hpp"

namespace by {

    BY(DEF_ME(baseFuncType))

    const impliAses& me::_getImpliAses() const {
        static impliAses inner{new baseFuncAsBaseFunc()};
        return inner;
    }

    const ases& me::_getAses() const {
        static ases inner(*new baseFuncAsStr());
        return inner;
    }
}
