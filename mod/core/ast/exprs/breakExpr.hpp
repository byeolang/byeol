#pragma once

#include "retStateExpr.hpp"

namespace nm {

    class _nout breakRet : public retState {
        NAMU(CLASS(breakRet, retState))
    };

    class _nout breakExpr : public retStateExpr {
        NAMU(CLASS(breakExpr, retStateExpr, expr::exprType), VISIT())

    public:
        str getEval() const override;
    };
}
