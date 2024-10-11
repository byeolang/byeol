#pragma once

#include "convergence.hpp"

namespace nm {

    class param;
    class node;
    class func;

    class paramConvergence: public convergence {
        NM(CLASS(paramConvergence, convergence))

    public:
        paramConvergence(baseFunc& f, param& p, const node& org);

    public:
        nbool converge() const override;

    private:
        baseFunc* _f;
        param* _p;
        const node* _org;
    };
}
