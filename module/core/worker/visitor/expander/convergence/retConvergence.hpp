#pragma once

#include "core/worker/visitor/expander/convergence/evalConvergence.hpp"

namespace by {
    class baseFunc;
    class node;

    class retConvergence: public evalConvergence {
        BY(CLASS(retConvergence, evalConvergence))

    public:
        retConvergence(baseObj& obj, baseFunc& f, const node& org);
    };
}
