/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Assignment expression for variable assignment
    /// @details Represents assignment operations (e.g., x = value).
    /// Handles variable binding and value assignment in the current scope.
    class _nout assignExpr: public expr {
        BY(CLASS(assignExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;
        typedef scope::iter iter;

    public:
        assignExpr(const node& lhs, const node& rhs);

    public:
        using super::run;
        str run(const args& a) override;
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
