/// @file
#pragma once

#include "core/builtin/scalar/scalar.hpp"
#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Fundamental Unary Operator expression
    /// @details Handles unary operator expressions. One operand goes into args and must be a @ref scalar type.
    class _nout FUOExpr: public expr {
        BY(CLASS(FUOExpr, expr, expr::exprType), VISIT())

    public:
        enum symbol {
            SYMBOL_POSTFIX_DOUBLE_PLUS = 0,
            SYMBOL_START = SYMBOL_POSTFIX_DOUBLE_PLUS,
            SYMBOL_POSTFIX_DOUBLE_MINUS,
            SYMBOL_BITWISE_NOT,
            SYMBOL_END
        };

    public:
        FUOExpr(symbol s, const node& it);

    public:
        using super::eval;
        str eval(const args& a) override;

        node& getOperand();
        const node& getOperand() const BY_CONST_FUNC(getOperand())
        str infer() const override;
        symbol getSymbol() const;
        static const nchar* getSymbolName(symbol r);

    private:
        symbol _symbol;
        str _it;
    };
} // namespace by
