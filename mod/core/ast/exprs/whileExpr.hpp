#pragma once

#include "loopExpr.hpp"

namespace nm {

    class _nout whileExpr : public loopExpr {
        NM(CLASS(whileExpr, loopExpr, expr::exprType), VISIT())
        friend class verifier;

    public:
        whileExpr(const node& condition, const blockExpr& blk);

    public:
        const node& getCondition() const NM_CONST_FUNC(getCondition())
        node& getCondition();

    protected:
        tstr<loop> _makeLoop(arr& ret) const override;

    private:
        str _condition;
    };
}
