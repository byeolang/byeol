#pragma once

#include "../expr.hpp"

namespace namu {

    class visitor;
    class errReport;

    class _nout blockExpr : public expr {
        NAMU(CLASS(blockExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        blockExpr();
        template <typename... Es>
        blockExpr(const Es&... elems): _exprs(elems...) {}

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const narr& getStmts() const;
        narr& getStmts();

        using super::inFrame;
        void inFrame(const bicontainable& args) override;
        void outFrame() override;

        clonable* cloneDeep() const override;

    private:
        narr _exprs;
    };
}
