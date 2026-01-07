/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {

    class visitor;
    class errReport;

    /// @ingroup core
    /// @brief Block expression
    /// @details Handles block expressions. It's frameInteractable, and when frame registration is requested
    /// externally, it creates and registers one scope. This scope is valid only within this block statement,
    /// commonly known as a local scope.
    ///
    /// @remark blockExpr doesn't create a local scope inside the eval() function. This optimizes by preventing
    /// unnecessary scope creation in certain contexts.
    ///
    /// @section blocks_are_expressions Blocks are Expressions
    /// The byeol language is mostly composed of expressions, and block statements are no exception. A block
    /// statement returns the evaluation result of its last line.
    class _nout blockExpr: public expr {
        BY(CLASS(blockExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        blockExpr();

        template <typename... Es> blockExpr(const Es&... elems): _exprs(elems...) {}

    public:
        using super::eval;
        str eval(const args& a) override;

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
