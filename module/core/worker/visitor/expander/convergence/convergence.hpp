/// @file
#pragma once

#include "core/worker/visitor/expander/convergence/tbaseConvergence.hpp"

namespace by {
    class convergence: public tbaseConvergence<void> {
        BY(CLASS(convergence, tbaseConvergence))

    public:
        convergence(baseObj& obj, baseFunc& fun, onConverge closure);

    public:
        nbool convergeWithoutFrame() override;

    private:
        onConverge _onConverge;
    };
}
