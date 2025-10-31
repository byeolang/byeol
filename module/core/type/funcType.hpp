/// @file
#pragma once

#include "core/type/mgdType.hpp"

namespace by {
    /// @ingroup core
    /// @brief Managed type for function objects
    /// @details Specialized type information for function objects with implicit conversion support.
    class _nout funcType: public mgdType {
        BY(ME(funcType, mgdType), CLONE(funcType))

    public:
        funcType(const std::string& typeName, const type& super, const params& ps, nbool isAdt, const node* ret);

    protected:
        const impliAses& _getImpliAses() const override;
        const ases& _getAses() const override;
    };
}
