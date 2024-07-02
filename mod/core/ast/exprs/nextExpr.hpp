#pragma once

#include "retStateExpr.hpp"

namespace nm {

    class _nout nextRet : public retState {
        NAMU(CLASS(nextRet, retState))
    };

    class _nout nextExpr : public retStateExpr {
        NAMU(CLASS(nextExpr, retStateExpr, expr::exprType), VISIT())

    public:
        str getEval() const override;
    };
}
