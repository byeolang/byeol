#pragma once

#include "tbaseConvergence.hpp"

namespace nm {
    class convergence : public tbaseConvergence<void> {
        NM(CLASS(convergence, tbaseConvergence))

    public:
        convergence(baseObj& obj, baseFunc& fun, onConverge closure);

    public:
        nbool convergeWithoutFrame() override;

    private:
        onConverge _onConverge;
    };
}
