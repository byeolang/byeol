/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Assignment expression
     *  @details Handles assignment expressions.
     *
     *  @remark This expression changes the reference registered in the scope, not calling operator=() on the object
     *  itself. For instance, calling obj::operator=() behaves like a deep copy, so be careful.
     */
    class _nout assignExpr: public expr {
        BY(CLASS(assignExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;
        typedef scope::iter iter;

    public:
        assignExpr(const node& lhs, const node& rhs);

    public:
        using super::eval;
        str eval(const args& a) override;
        str infer() const override;
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
