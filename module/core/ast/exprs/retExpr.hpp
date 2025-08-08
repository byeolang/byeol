#pragma once

#include "core/ast/exprs/retStateExpr.hpp"

namespace by {

    class visitor;

    class _nout retExpr: public retStateExpr {
        NM(CLASS(retExpr, retStateExpr, expr::exprType), VISIT())

    public:
        retExpr(const node& ret);
        retExpr();

    public:
        using super::run;
        str run(const args& a) override;

        node& getRet();
        const node& getRet() const NM_CONST_FUNC(getRet())

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        str getEval() const override;

    private:
        nbool _isEx(const node& got, const node& funcRet);

    private:
        str _ret;
    };
}
