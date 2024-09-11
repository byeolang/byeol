#pragma once

#include "../baseFunc.hpp"

namespace nm {

    class baseCtor: public baseFunc {
        NM(ME(baseCtor, baseFunc), VISIT())
        friend class generalizer; // for _setOrigin()

    public:
        baseCtor(const node& org);

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
