#pragma once

#include "core/worker/visitor/expander/convergence/evalConvergence.hpp"

namespace by {

    class param;
    class node;
    class func;

    class paramConvergence: public evalConvergence {
        BY(CLASS(paramConvergence, evalConvergence))

    public:
        paramConvergence(baseObj& o, baseFunc& f, param& p, const node& org);

    private:
        param* _p;
    };
}
