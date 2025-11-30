/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {

    class visitor;
    class errReport;

    /// @ingroup core
    /// @brief Block expression containing multiple statements
    /// @details Represents a sequence of statements executed in order.
    /// Used for function bodies, control flow blocks, and compound statements.
    class _nout blockExpr: public expr {
        BY(CLASS(blockExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        blockExpr();

        template <typename... Es> blockExpr(const Es&... elems): _exprs(elems...) {}

    public:
        using super::run;
        str run(const args& a) override;

        str infer() const override;

        void setEval(const node& newEval);
        void setEval(const node* it) BY_SIDE_FUNC(setEval);

        const narr& getStmts() const BY_CONST_FUNC(getStmts())
        narr& getStmts();

        void rel() override;

        using super::inFrame;
        void inFrame(const bicontainable* args) const override;

        using super::outFrame;
        void outFrame() const override;

        void onCloneDeep(const clonable& from) override;

        nbool isAbstract() const;

    private:
        narr _exprs;
        str _eval;
    };
} // namespace by
