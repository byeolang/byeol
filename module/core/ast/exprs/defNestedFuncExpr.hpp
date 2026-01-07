/// @file
#pragma once

#include "core/ast/func.hpp"
#include "core/ast/expr.hpp"

namespace by {
    /// @ingroup core
    /// @brief Nested function definition expression
    /// @details Creates byeol language nested functions @ref nestedFunc. Nested functions can omit the name when
    /// defined and can also be used as @ref closure.
    class _nout defNestedFuncExpr: public expr {
        BY(CLASS(defNestedFuncExpr, expr, expr::exprType), VISIT())
        friend class visitor;

    public:
        defNestedFuncExpr(const func& org);

    public:
        using super::eval;
        str eval(const args& a) override;

        const func& getOrigin() const BY_CONST_FUNC(_getOrigin())

        str infer() const override;

    private:
        scope* _cloneLocalScope(frame& fr) const;
        func& _getOrigin();

    private:
        tstr<func> _org;
    };
}
