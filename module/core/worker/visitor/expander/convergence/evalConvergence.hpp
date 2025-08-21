/// @file
#pragma once

#include "core/worker/visitor/expander/convergence/tbaseConvergence.hpp"

namespace by {
    class evalConvergence: public tbaseConvergence<node> {
        BY(ADT(evalConvergence, tbaseConvergence))

    public:
        evalConvergence(baseObj& obj, baseFunc& fun, const node& org, onConverge closure);

    public:
        nbool convergeWithoutFrame() override;

    private:
        str _org;
    };
}
