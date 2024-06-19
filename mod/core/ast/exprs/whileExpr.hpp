#pragma once

#include "loopExpr.hpp"

namespace namu {

    class _nout whileExpr : public loopExpr {
        NAMU(CLASS(whileExpr, loopExpr, expr::exprType), VISIT())
        friend class verifier;

    public:
        whileExpr(const node& condition, const blockExpr& blk);

    public:
        const node& getCondition() const NAMU_UNCONST_FUNC(getCondition())
        node& getCondition();

        str getEval() const override;

    protected:
        tstr<loop> _makeLoop(arr& ret) const override;

    private:
        str _condition;
        mutable nbool _initEval;
    };
}
