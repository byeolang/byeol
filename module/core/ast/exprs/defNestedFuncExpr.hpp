/// @file
#pragma once

#include "core/ast/func.hpp"
#include "core/ast/expr.hpp"

namespace by {
    /** @ingroup core
     *  @brief Nested function definition expression
     *  @details Creates byeol language nested functions @ref nestedFunc. Nested functions can omit the name when
     *  defined and can also be used as @ref closure.
     */
    class _nout defNestedFuncExpr: public expr {
        BY(CLASS(defNestedFuncExpr, expr, expr::exprType), VISIT())

    public:
        defNestedFuncExpr(const func& org);

    public:
        using super::eval;
        str eval(const args& a) override;

        func& getOrigin();
        const func& getOrigin() const BY_CONST_FUNC(getOrigin())

        str infer() const override;

    private:
        scope* _cloneLocalScope(frame& fr) const;

    private:
        tstr<func> _org;
    };
}
