#pragma once

#include "evalConvergence.hpp"

namespace nm {

    class param;
    class node;
    class func;

    class paramConvergence: public evalConvergence {
        NM(CLASS(paramConvergence, evalConvergence))

    public:
        paramConvergence(baseObj& o, baseFunc& f, param& p, const node& org);

    private:
        param* _p;
    };
}
