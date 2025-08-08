#pragma once

#include "core/builtin/primitive/arithmeticObj.hpp"
#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class visitor;

    // FAO: Fundermental Unary Operation.
    class _nout FUOExpr: public expr {
        NM(CLASS(FUOExpr, expr, expr::exprType), VISIT())

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
        using super::run;
        str run(const args& a) override;

        node& getOperand();
        const node& getOperand() const NM_CONST_FUNC(getOperand())
        str getEval() const override;
        symbol getSymbol() const;
        static const nchar* getSymbolName(symbol r);

    private:
        symbol _symbol;
        str _it;
    };
} // namespace by
