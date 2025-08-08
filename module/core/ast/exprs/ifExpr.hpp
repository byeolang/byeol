#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/exprs/blockExpr.hpp"

namespace by {

    class _nout ifExpr: public expr {
        NM(CLASS(ifExpr, expr, expr::exprType), VISIT())
        friend class verifier;

    public:
        ifExpr(const node& exp, const blockExpr& thenBlk);
        ifExpr(const node& exp, const blockExpr& thenBlk, const blockExpr& elseBlk);

    public:
        blockExpr& getThen();
        const blockExpr& getThen() const BY_CONST_FUNC(getThen())
        void setThen(const blockExpr& newThen);
        void setThen(const blockExpr* it) BY_SIDE_FUNC(setThen);

        blockExpr* getElse();
        const blockExpr* getElse() const BY_CONST_FUNC(getElse())
        void setElse(const blockExpr& newElse);
        void setElse(const blockExpr* it) BY_SIDE_FUNC(setElse);

        node& getCondition();
        const node& getCondition() const BY_CONST_FUNC(getCondition())
        void setCondition(const node& newCondition);
        void setCondition(const node* it) BY_SIDE_FUNC(setCondition);

        using super::run;
        str run(const args& a) override;

        str getEval() const override;

    private:
        str _expr;
        tstr<blockExpr> _then;
        tstr<blockExpr> _else;
    };
}
