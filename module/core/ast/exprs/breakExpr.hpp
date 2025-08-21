/// @file
#pragma once

#include "core/ast/exprs/retStateExpr.hpp"

namespace by {

    /// @ingroup core
    /// @brief Return state for break statements
    /// @details Represents the return state when a break statement is executed.
    class _nout breakRet: public retState {
        BY(CLASS(breakRet, retState))
    };

    /// @ingroup core
    /// @brief Break statement expression
    /// @details Represents break statements that exit loops early.
    /// Used to terminate loop execution and return control to outer scope.
    class _nout breakExpr: public retStateExpr {
        BY(CLASS(breakExpr, retStateExpr, expr::exprType), VISIT())

    public:
        str getEval() const override;
    };
}
