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
    /// @brief Base class for keywords that exit @ref blockExpr
    /// @details Base class for keywords that break out of @ref blockExpr.
    ///
    /// @section block_exit_algorithm Algorithm for Immediately Exiting Block Statements
    /// When a block statement must be exited immediately via `ret` etc., the value must be placed in @ref thread via
    /// `setRet()`. Each blockExpr executes statements line by line, and when setRet() detects a certain value in the
    /// thread, it stops all operations and passes that value up to the block statement's caller.
    ///
    /// An alternative approach could check if the expr's `eval()` return value is retStateExpr. However, in this
    /// case, all exprs owning blockExpr (@ref ifExpr, @ref forExpr, etc.) must each include code to immediately pass
    /// it up when their owned blockExpr's result is retStateExpr. Otherwise, nested blockExpr cases (blockExpr
    /// inside ifExpr inside blockExpr) cannot be handled.
    class _nout retStateExpr: public expr {
        BY(ADT(retStateExpr, expr), VISIT())

    public:
        using super::eval;
        str eval(const args& a) override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;
    };
}
