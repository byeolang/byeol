#pragma once

#include "convergence.hpp"

namespace nm {
    class baseFunc;
    class node;

    class retConvergence: public convergence {
        NM(CLASS(retConvergence, convergence))

    public:
        retConvergence(baseObj& obj, baseFunc& f, const node& org);

    private:
        str _org;
    };
}
