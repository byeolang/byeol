/// @file
#pragma once

#include "core/ast/exprs/loopExpr.hpp"

namespace by {

    /** @ingroup core
     *  @brief While loop expression
     *  @details Represents while loop constructs with condition-based iteration.
     *  Executes loop body while the condition evaluates to true.
     */
    class _nout whileExpr: public loopExpr {
        BY(CLASS(whileExpr, loopExpr, expr::exprType), VISIT())

    public:
        whileExpr(const node& condition, const blockExpr& blk);

    public:
        const node& getCondition() const BY_CONST_FUNC(getCondition())
        node& getCondition();

    protected:
        tstr<loop> _makeLoop(arr* ret) const override;

    private:
        str _condition;
    };
}
