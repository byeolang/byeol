/// @file
#pragma once

#include "core/ast/exprs/retStateExpr.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Return statement expression
     *  @details Represents return statements that exit @ref func with optional values.
     *  Handles return value evaluation and function termination.
     */
    class _nout retExpr: public retStateExpr {
        BY(CLASS(retExpr, retStateExpr, expr::exprType), VISIT())

    public:
        retExpr(const node& ret);
        retExpr();

    public:
        using super::eval;
        str eval(const args& a) override;

        node& getRet();
        const node& getRet() const BY_CONST_FUNC(getRet())

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        str infer() const override;

    private:
        /**
         * @brief Internal helper to check for exception conditions related to the return value.
         * @details This method determines if the return value (`got`) indicates an exception
         *          when compared against the function's declared return type (`funcRet`).
         * @param got The actual value being returned.
         * @param funcRet The declared return type of the function.
         * @return true if an exception condition is detected, false otherwise.
         */
        nbool _isEx(const node& got, const node& funcRet);

    private:
        str _ret;
    };
}
