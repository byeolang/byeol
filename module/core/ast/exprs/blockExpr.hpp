#pragma once

#include "core/ast/expr.hpp"

namespace nm {

    class visitor;
    class errReport;

    class _nout blockExpr: public expr {
        NM(CLASS(blockExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        blockExpr();

        template <typename... Es> blockExpr(const Es&... elems): _exprs(elems...) {}

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        void setEval(const node& newEval);
        const narr& getStmts() const NM_CONST_FUNC(getStmts())
        narr& getStmts();
        void rel() override;

        using super::inFrame;
        void inFrame(const bicontainable& args) const override;
        using super::outFrame;
        void outFrame(const bicontainable& args) const override;

        void onCloneDeep(const clonable& from) override;

        nbool isAbstract() const;

    private:
        narr _exprs;
        str _eval;
    };
} // namespace nm
