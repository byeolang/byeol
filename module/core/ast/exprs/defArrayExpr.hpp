#pragma once

#include "core/builtin/container/mgd/arr.hpp"
#include "core/ast/expr.hpp"

namespace by {

    class _nout defArrayExpr: public expr {
        NM(CLASS(defArrayExpr, expr, expr::exprType), VISIT())
        friend class visitor; // for getElems()

    public:
        defArrayExpr(const node& type);
        defArrayExpr(const narr& elems);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;
        const baseObj* getArrayType() const;
        const arr& getOrigin() const;
        const narr& getElems() const BY_CONST_FUNC(getElems())

    private:
        tstr<baseObj> _deduceElems() const;
        narr& getElems();

    private:
        str _type;
        narr _elems;
        mutable tstr<arr> _org;
    };
}
