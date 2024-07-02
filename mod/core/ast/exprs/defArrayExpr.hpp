#pragma once

#include "../expr.hpp"
#include "../../builtin/container/mgd/arr.hpp"

namespace nm {

    class _nout defArrayExpr : public expr {
        NM(CLASS(defArrayExpr, expr, expr::exprType), VISIT())
        friend class visitor; // for getElems()

    public:
        defArrayExpr(const node& type);
        defArrayExpr(const narr& elems);

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const node& getArrayType() const;
        const arr& getOrigin() const;
        const narr& getElems() const NM_CONST_FUNC(getElems())

    private:
        str _deduceElems() const;
        narr& getElems();

    private:
        str _type;
        narr _elems;
        mutable tstr<arr> _org;
    };
}
