/// @file
#pragma once

#include "core/ast/baseFunc.hpp"

namespace by {

    /// @ingroup core
    /// @brief Base class for all constructor types
    /// @details Abstract base class for constructors providing common functionality.
    /// Manages constructor type information and origin object relationships.
    class baseCtor: public baseFunc {
        BY(ME(baseCtor, baseFunc), VISIT())
        friend class generalizer; // for _setOrigin()

    public:
        baseCtor(const mgdType& type);

    public:
        str getEval() const override;
        const ntype& getType() const override;
        const baseObj& getOrigin() const override;

    private:
        void _setOrigin(const baseObj& newOrg);

    private:
        mgdType _type;
    };
}
