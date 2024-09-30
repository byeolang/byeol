#pragma once

#include "convergence.hpp"

namespace nm {

    class param;
    class node;
    class func;

    class paramConvergence : public convergence {
        NM(CLASS(paramConvergence, convergence))

    public:
        paramConvergence(func& f, param& p, const node& org);

    public:
        nbool converge() const override;

    private:
        func* _f;
        param* _p;
        const node* _org;
    };
}
