#pragma once

#include "baseFunc.hpp"

namespace namu {

    class ctor: public baseFunc {
        NAMU(ADT(ctor, baseFunc), VISIT())
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
