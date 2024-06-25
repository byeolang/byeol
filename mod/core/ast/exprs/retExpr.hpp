#pragma once

#include "retStateExpr.hpp"

namespace namu {

    class visitor;

    class _nout retExpr : public retStateExpr {
        NAMU(CLASS(retExpr, retStateExpr, expr::exprType), VISIT())

    public:
        retExpr(const node& ret);
        retExpr();

    public:
        using super::run;
        str run(const args& a) override;

        node& getRet();
        const node& getRet() const NAMU_UNCONST_FUNC(getRet())
        priorType prioritize(const args& a) const override;
        str getEval() const override;

    private:
        nbool _isEx(const node& got, const node& funcRet);

    private:
        str _ret;
    };
}
