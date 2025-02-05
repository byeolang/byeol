#pragma once

#include "tbaseConvergence.hpp"

namespace nm {
    class evalConvergence : public tbaseConvergence<node> {
        NM(ADT(evalConvergence, tbaseConvergence))

    public:
        evalConvergence(baseObj& obj, baseFunc& fun, const node& org, onConverge closure);

    public:
        nbool convergeWithoutFrame() override;

    private:
        str _org;
    };
}
