/// @file
#pragma once

#include "core/type/mgdType.hpp"

namespace by {
    /// @ingroup core
    /// @brief Managed type for function objects
    /// @details Specialized type information for function objects with implicit conversion support.
    class _nout funcMgdType: public mgdType {
        BY(ME(funcMgdType, mgdType), CLONE(funcMgdType))

    public:
        funcMgdType(const std::string& name, const type& super, const params& ps, nbool isAdt, const node* ret);

    protected:
        const impliAses& _getImpliAses() const override;
    };
}
