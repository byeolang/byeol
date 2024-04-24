#pragma once

#include "retStateExpr.hpp"

namespace namu {

    class _nout nextRet : public retState {
        NAMU(CLASS(nextRet, retState))
    };

    class _nout nextExpr : public retStateExpr {
        NAMU(CLASS(nextExpr, retStateExpr, expr::exprType), VISIT())

    protected:
        const node& _onGetRet() const override;
    };
}
