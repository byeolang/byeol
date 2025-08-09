#pragma once

#include "core/ast/exprs/retStateExpr.hpp"

namespace by {

    class _nout nextRet: public retState {
        BY(CLASS(nextRet, retState))
    };

    class _nout nextExpr: public retStateExpr {
        BY(CLASS(nextExpr, retStateExpr, expr::exprType), VISIT())

    public:
        str getEval() const override;
    };
}
