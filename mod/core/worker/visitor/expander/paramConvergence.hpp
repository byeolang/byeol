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

    protected:
        nbool _onConverge(baseFunc& f) const override;

    private:
        param* _p;
        str _org;
    };
}
