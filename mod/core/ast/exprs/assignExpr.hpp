#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace nm {

    class visitor;

    class _nout assignExpr: public expr {
        NM(CLASS(assignExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;
        typedef scope::iter iter;

    public:
        assignExpr(const node& lhs, const node& rhs);

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const node& getLeft() const;
        const node& getRight() const;
        void onCloneDeep(const clonable& from) override;

    private:
        iter _getScopeIterOfLhs();

    private:
        str _lhs; // name of variable or lambda.
        str _rhs;
    };
}
