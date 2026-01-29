/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {

    class visitor;
    class errReport;

    /** @ingroup core
     *  @brief Block expression
     *  @details Handles block expressions. It's frameInteractable, and when frame registration is requested
     *  externally, it creates and registers one scope. This scope is valid only within this block statement,
     *  commonly known as a local scope.
     *
     *  @remark blockExpr doesn't create a local scope inside the eval() function. This optimizes by preventing
     *  unnecessary scope creation in certain contexts.
     *
     *  @section blocks_are_expressions Blocks are Expressions
     *  The byeol language is mostly composed of expressions, and block statements are no exception. A block
     *  statement returns the evaluation result of its last line.
     */
    class _nout blockExpr: public expr {
        BY(CLASS(blockExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        blockExpr();

        /**
         * @brief Constructs a blockExpr with an initializer list of child expressions (statements).
         * @details This variadic template constructor allows convenient initialization of the
         *          block with multiple statements.
         * @tparam Es Types of the child expressions.
         * @param elems The child expressions (statements) to add to this block.
         */
        template <typename... Es> blockExpr(const Es&... elems): _exprs(elems...) {}

    public:
        using super::eval;
        str eval(const args& a) override;

        str infer() const override;

        /**
         * @brief Sets the node whose evaluation result will be returned by this block expression.
         * @param newEval The node representing the evaluation result of this block.
         */
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

        /**
         * @brief Checks if this block expression is abstract.
         * @return true if the block expression is abstract, false otherwise.
         * @note An abstract block typically means it lacks a concrete implementation or return value.
         */
        nbool isAbstract() const;

    private:
        narr _exprs;
        str _eval;
    };
} // namespace by
