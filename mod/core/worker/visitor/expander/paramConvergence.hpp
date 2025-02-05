#pragma once

#include "convergence.hpp"

namespace nm {

    class param;
    class node;
    class func;

    class paramConvergence: public convergence {
        NM(CLASS(paramConvergence, convergence))

    public:
        paramConvergence(baseObj& o, baseFunc& f, param& p, const node& org);

    private:
        param* _p;
    };
}
