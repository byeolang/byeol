#pragma once

#include "core/ast/func.hpp"
#include "core/ast/expr.hpp"

namespace by {
    /// nested func:
    ///     when you eval nested func, actually it will clone a new func with the current
    ///     shallow copied block scope. so it can captures local scope at that time.
    ///
    ///     it doesn't know about local variables after you define it.
    ///     it contains copied block scope. and will inject just before runs block stmt.
    class _nout defNestedFuncExpr: public expr {
        BY(CLASS(defNestedFuncExpr, expr, expr::exprType), VISIT())
        friend class visitor;

    public:
        defNestedFuncExpr(const func& org);

    public:
        using super::run;
        str run(const args& a) override;

        const func& getOrigin() const BY_CONST_FUNC(_getOrigin())

        str getEval() const override;

    private:
        scope* _cloneLocalScope(frame& fr) const;
        func& _getOrigin();

    private:
        tstr<func> _org;
    };
}
