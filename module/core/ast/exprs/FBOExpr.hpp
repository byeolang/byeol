/// @file
#pragma once

#include "core/builtin/primitive/arithmeticObj.hpp"
#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Fundamental Binary Operation expression
    /// @details Represents binary operations like +, -, *, /, ==, <, etc.
    /// Handles arithmetic, bitwise, and logical binary operations between two operands.
    class _nout FBOExpr: public expr {
        BY(CLASS(FBOExpr, expr, expr::exprType), VISIT())

    public:
        enum symbol {
            SYMBOL_ADD = 0,
            SYMBOL_START = SYMBOL_ADD,
            SYMBOL_ARITH_START = SYMBOL_START,
            SYMBOL_SUB,
            SYMBOL_MUL,
            SYMBOL_DIV,
            SYMBOL_MOD,
            SYMBOL_BITWISE_AND,
            SYMBOL_BITWISE_XOR,
            SYMBOL_BITWISE_OR,
            SYMBOL_LSHIFT,
            SYMBOL_RSHIFT,
            SYMBOL_ARITH_END,

            SYMBOL_EQ,
            SYMBOL_LOGIC_START = SYMBOL_EQ,
            SYMBOL_NE,
            SYMBOL_GT,
            SYMBOL_LT,
            SYMBOL_GE,
            SYMBOL_LE,
            SYMBOL_AND,
            SYMBOL_OR,
            SYMBOL_LOGIC_END,
            SYMBOL_END = SYMBOL_LOGIC_END
        };

    public:
        FBOExpr(symbol s, const node& lhs, const node& rhs);

    public:
        using super::eval;
        str eval(const args& a) override;

        str infer() const override;
        const node& getLeft() const;

        void setLeft(const node& new1);
        void setLeft(const node* it) BY_SIDE_FUNC(setLeft);

        const node& getRight() const;

        void setRight(const node& new1);
        void setRight(const node* it) BY_SIDE_FUNC(setRight);

        nbool isLogicalOp() const;
        symbol getSymbol() const;
        static const nchar* getSymbolName(symbol s);

    private:
        symbol _symbol;
        str _lhs;
        str _rhs;
    };
} // namespace by
