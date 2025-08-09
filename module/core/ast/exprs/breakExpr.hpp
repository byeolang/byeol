#pragma once

#include "core/ast/exprs/retStateExpr.hpp"

namespace by {

    class _nout breakRet: public retState {
        BY(CLASS(breakRet, retState))
    };

    class _nout breakExpr: public retStateExpr {
        BY(CLASS(breakExpr, retStateExpr, expr::exprType), VISIT())

    public:
        str getEval() const override;
    };
}
