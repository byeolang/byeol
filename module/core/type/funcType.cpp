#include "core/type/funcType.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/builtin/primitive/nStr.hpp"
#include "core/type/as/baseFuncAses.hpp"

namespace by {
    BY(DEF_ME(funcType))

    me::funcType(const std::string& typeName, const type& superType, const params& ps, nbool isAdt, const node* ret):
        super(typeName, superType, ps, isAdt, ret) {}

    const impliAses& me::_getImpliAses() const {
        static impliAses inner{new baseFuncAsBaseFunc()};
        return inner;
    }

    const ases& me::_getAses() const {
        static ases inner(*new baseFuncAsStr());
        return inner;
    }
} // namespace by
