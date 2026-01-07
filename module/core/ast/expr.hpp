/// @file
#pragma once

#include "core/ast/args.hpp"
#include "core/ast/src/src.hpp"

struct exprTest;

namespace by {

    /// @ingroup core
    /// @brief Base class for all expressions
    /// @details Abstract base class for all expression nodes in the AST. Provides evaluation semantics and
    /// expression-specific type handling. Expressions represent computations that produce values, such as arithmetic
    /// operations, function calls, variable access, and assignments.
    ///
    /// @section expression_hierarchy Expression Hierarchy
    /// All expression types inherit from expr:
    /// - **Literal expressions** - @ref nInt, @ref nStr, @ref nBool, etc.
    /// - **Operator expressions** - @ref FBOExpr (binary), @ref FUOExpr (unary)
    /// - **Access expressions** - @ref getExpr, @ref evalExpr
    /// - **Assignment expressions** - @ref assignExpr, @ref defVarExpr, @ref defPropExpr
    /// - **Control flow expressions** - @ref ifExpr, @ref forExpr, @ref whileExpr, @ref retExpr
    /// - **Container expressions** - @ref defArrayExpr, @ref defSeqExpr
    /// - **Type expressions** - @ref asExpr, @ref isExpr
    ///
    /// @section evaluation_without_arguments Evaluation Without Arguments
    /// Unlike functions, expressions evaluate without taking arguments. The `prioritize()` method ensures expressions
    /// can only be evaluated with an empty argument list. Evaluation happens through `eval()` which computes and
    /// returns the expression's value.
    ///
    /// @section source_location Source Location
    /// expr maintains source location information via `getSrc()` for error reporting and debugging. This allows the
    /// interpreter to report exactly where in the source code an expression appears when errors occur.
    class _nout expr: public node {
    public:
        // expr can be casted to @ref node. it's proxy.
        /// @ingroup core
        /// @brief @ref ntype "Type" information for expression nodes
        /// @details Provides @ref ntype "type" metadata and conversion rules for
        /// @ref expr "expressions".
        class _nout exprType: public ntype {
            BY(ME(exprType, ntype))

        public:
            nbool isImpli(const type& to) const override;

            using super::asImpli;
            str asImpli(const node& from, const type& to) const override;
        };

        BY(ADT(expr, node, exprType))
        friend class exprMaker;
        friend struct ::exprTest;

    public:
        using super::subs;
        scope& subs() override;

        using super::isImpli;
        nbool isImpli(const type& to) const override;

        using super::prioritize;
        /// eval of @ref expr class get treated like 'evaluate' in byeol.
        /// it should not take any @ref args "arguments" to eval
        priorType prioritize(const args& a) const override;

        const src& getSrc() const override;

    private:
        void _setSrc(const src& newSrc) override;

    protected:
        tstr<src> _src;
        point _pos;
    };
} // namespace by
