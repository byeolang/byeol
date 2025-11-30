/// @file
#pragma once

#include "core/ast/func.hpp"
#include "core/ast/expr.hpp"

namespace by {
    /// @ingroup core
    /// @brief Nested @ref func definition expression (lambda)
    /// @details Represents lambda expressions and nested function definitions.
    /// Captures local scope at definition time and creates @ref closure for nested functions.
    /// @note When evaluated, clones function with shallow copied block scope for variable capture.
    class _nout defNestedFuncExpr: public expr {
        BY(CLASS(defNestedFuncExpr, expr, expr::exprType), VISIT())
        friend class visitor;

    public:
        defNestedFuncExpr(const func& org);

    public:
        using super::run;
        str run(const args& a) override;

        const func& getOrigin() const BY_CONST_FUNC(_getOrigin())

        str infer() const override;

    private:
        scope* _cloneLocalScope(frame& fr) const;
        func& _getOrigin();

    private:
        tstr<func> _org;
    };
}
