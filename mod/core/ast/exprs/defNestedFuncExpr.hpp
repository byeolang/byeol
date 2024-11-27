#pragma once

#include "../func.hpp"
#include "../expr.hpp"

namespace nm {
    /// nested func:
    ///     when you eval nested func, actually it will clone a new func with the current
    ///     shallow copied block scope. so it can captures local scope at that time.
    ///
    ///     it doesn't know about local variables after you define it.
    ///     it contains copied block scope. and will inject just before runs block stmt.
    class _nout defNestedFuncExpr : public expr {
        NM(CLASS(defNestedFuncExpr, expr))

    public:
        defNestedFuncExpr(const func& org);

    public:
        using super::run;
        str run(const args& a) override;

        const func& getOrigin() const;

        str getEval() const override;

    private:
        tstr<func> _org;
    };
}
