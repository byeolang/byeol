#pragma once

#include "../expr.hpp"
#include "../../builtin/primitive/nVoid.hpp"

namespace nm {

    class _nout retState : public nVoid {
        NM(CLASS(retState, nVoid))
    };

    class _nout retStateExpr : public expr {
        NM(ADT(retStateExpr, expr), VISIT())

    public:
        using super::run;
        str run(const args& a) override;

        priorType prioritize(const args& a) const override;
    };
}
