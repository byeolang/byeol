#pragma once

#include "convergence.hpp"

namespace nm {
    class baseFunc;
    class node;

    class retConvergence: public convergence {
        NM(CLASS(retConvergence, convergence))

    public:
        retConvergence(baseObj& obj, baseFunc& f, const node& org);

    protected:
        nbool _onConverge(baseFunc& f) const override;

    private:
        const node* _org;
    };
}
