/// @file
#pragma once

#include "core/ast/exprs/retStateExpr.hpp"

namespace by {

    /** @ingroup core
     *  @brief Return state for continue statements
     *  @details Represents the return state when a continue statement is executed.
     */
    class _nout nextRet: public retState {
        BY(CLASS(nextRet, retState))
    };

    /** @ingroup core
     *  @brief Continue statement expression
     *  @details Represents continue statements that skip to next loop iteration.
     *  Used to continue loop execution from the beginning of the current iteration.
     */
    class _nout nextExpr: public retStateExpr {
        BY(CLASS(nextExpr, retStateExpr, expr::exprType), VISIT())

    public:
        str infer() const override;
    };
}
