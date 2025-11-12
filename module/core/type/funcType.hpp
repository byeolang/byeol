/// @file
#pragma once

#include "core/type/mgdType.hpp"

namespace by {
    /// @ingroup core
    /// @brief Managed type for function objects in byeol language.
    /// @details Specialized type information for function objects with implicit conversion support.
    class _nout funcType: public mgdType {
        BY(ME(funcType, mgdType), CLONE(funcType))

    public:
        funcType(const std::string& typeName, const type& super, const params& ps, nbool isAdt, const node* ret);

    public:
        /// @return true if rhs has same params and return type in managed code
        /// @details operator==() only compares parameter and return type equality, but not the name.
        ///          because it's for 1st citizen feature of function type in managed code.
        nbool operator==(const type& rhs) const override;

    protected:
        const impliAses& _getImpliAses() const override;
        const ases& _getAses() const override;
    };
}
