#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../../builtin/primitive/arithmeticObj.hpp"
#include "../scope.hpp"

namespace nm {

    class visitor;

    // FAO: Fundermental Binary Operation.
    class _nout FBOExpr : public expr {
        NM(CLASS(FBOExpr, expr, expr::exprType), VISIT())

    public:
        enum rule {
            ADD = 0,
            START = ADD,
            ARITH_START = START,
            SUB, MUL, DIV, MOD, BITWISE_AND, BITWISE_XOR, BITWISE_OR, LSHIFT, RSHIFT,
            ARITH_END,

            EQ,
            LOGIC_START = EQ,
            NE, GT, LT, GE, LE, AND, OR, LOGIC_END,
            END = LOGIC_END
        };

    public:
        FBOExpr(rule rule, const node& lhs, const node& rhs);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;
        const node& getLeft() const;
        void setLeft(const node& new1);
        const node& getRight() const;
        void setRight(const node& new1);
        nbool isLogicalOp() const;
        rule getRule() const;
        static const nchar* getRuleName(rule r);

    private:
        rule _rule;
        str _lhs;
        str _rhs;
    };
}
