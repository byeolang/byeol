/// @file
#pragma once

#include "core/builtin/scalar/nVoid.hpp"
#include "core/ast/expr.hpp"

namespace by {

    /// @ingroup core
    /// @brief Return state object for control flow
    /// @details Represents return states for control flow statements.
    class _nout retState: public nVoid {
        BY(CLASS(retState, nVoid))
    };

    /// @ingroup core
    /// @brief Base class for return state expressions
    /// @details Abstract base class for expressions that change control flow (return, break, continue).
    class _nout retStateExpr: public expr {
        BY(ADT(retStateExpr, expr), VISIT())

    public:
        using super::eval;
        str eval(const args& a) override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;
    };
}
