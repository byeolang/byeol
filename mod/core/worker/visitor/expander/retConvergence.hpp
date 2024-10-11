#pragma once

#include "convergence.hpp"

namespace nm {
    class baseFunc;
    class node;

    class retConvergence: public convergence {
        NM(CLASS(retConvergence, convergence))

    public:
        retConvergence(baseFunc& f, const node& org);

    public:
        nbool converge() const override;

    private:
        baseFunc* _f;
        const node* _org;
    };
}
