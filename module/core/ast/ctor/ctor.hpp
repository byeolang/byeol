/// @file
#pragma once

#include "core/ast/func.hpp"

namespace by {
    /// @ingroup core
    /// @brief Constructor function for byeol objects
    /// @details Represents constructor functions that initialize object instances.
    /// Handles object construction, parameter passing, and initialization logic.
    class _nout ctor: public func {
        // don't define `CLASS` here:
        //  CLASS overrides getType() and it returns ttype<ctor> instead of mgdType
        //  from superclass
        BY(ME(ctor, func), CLONE(func), VISIT())

    public:
        ctor(const modifier& mod, const params& ps, const blockExpr& blk);

    public:
        using super::run;
        str run(const args& a) override;

        str infer() const override;

        const src& getSrc() const override;

    public:
        inline static const std::string CTOR_NAME = "@ctor";
    };
}
