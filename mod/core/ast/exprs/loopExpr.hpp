#pragma once

#include "../expr.hpp"
#include "blockExpr.hpp"

namespace namu {

    class frame;
    class arr;
    class _nout loopExpr : public expr {
        NAMU(ADT(loopExpr, expr, expr::exprType), VISIT())

    public:
        loopExpr(const blockExpr& blk);

    public:
        blockExpr& getBlock() const;
        str getEval() const override;
        nbool setEval(const node& new1) const;

        nbool isReturnable() const;
        void setReturnable(nbool isReturnable);

    protected:
        /// @return true if exit the loop
        nbool _postprocess() const;
        arr& _preprocess() const;

    private:
        mutable tstr<blockExpr> _blk;
        mutable str _eval;
        nbool _isReturnable;
    };
}
