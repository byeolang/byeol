/// @file
#pragma once

#include "core/builtin/primitive/nVoid.hpp"
#include "core/ast/expr.hpp"

namespace by {

    class _nout retState: public nVoid {
        BY(CLASS(retState, nVoid))
    };

    class _nout retStateExpr: public expr {
        BY(ADT(retStateExpr, expr), VISIT())

    public:
        using super::run;
        str run(const args& a) override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;
    };
}
