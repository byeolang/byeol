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
        nbool _isEx(const node& got, const node& funcRet);

    private:
        str _ret;
    };
}
