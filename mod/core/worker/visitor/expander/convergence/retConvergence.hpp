#pragma once

#include "evalConvergence.hpp"

namespace nm {
    class baseFunc;
    class node;

    class retConvergence: public evalConvergence {
        NM(CLASS(retConvergence, evalConvergence))

    public:
        retConvergence(baseObj& obj, baseFunc& f, const node& org);
    };
}
