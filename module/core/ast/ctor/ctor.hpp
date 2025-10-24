/// @file
#pragma once

#include "core/ast/func.hpp"
#include "core/type/ctorType.hpp"

namespace by {
    /// @ingroup core
    /// @brief Constructor function for byeol objects
    /// @details Represents constructor functions that initialize object instances.
    /// Handles object construction, parameter passing, and initialization logic.
    class _nout ctor: public func {
        BY(CLASS(ctor, func), VISIT())

    public:
        ctor(const modifier& mod, const params& ps, const blockExpr& blk);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;

        const src& getSrc() const override;
    };
}
