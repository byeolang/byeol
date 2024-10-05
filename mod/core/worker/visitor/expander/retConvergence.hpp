#pragma once

#include "convergence.hpp"

namespace nm {
    class func;
    class node;

    class retConvergence: public convergence {
        NM(CLASS(retConvergence, convergence))

    public:
        retConvergence(func& f, const node& org);

    public:
        nbool converge() const override;

    private:
        func* _f;
        const node* _org;
    };
}
