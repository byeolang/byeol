#pragma once

#include "baseFunc.hpp"

namespace nm {

    class ctor: public baseFunc {
        NM(ADT(ctor, baseFunc), VISIT())
        friend class generalizer; // for _setOrigin()

    public:
        ctor(const node& org);

    public:
        str getRet() const override;
        str getEval() const override;
        const node& getOrigin() const;

    private:
        void _setOrigin(const node& newOrg);

    private:
        str _org;
    };
}
