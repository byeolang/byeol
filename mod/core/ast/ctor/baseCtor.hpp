#pragma once

#include "../baseFunc.hpp"

namespace nm {

    class baseCtor: public baseFunc {
        NM(ME(baseCtor, baseFunc), VISIT())
        friend class generalizer; // for _setOrigin()

    public:
        baseCtor(const mgdType& type);

    public:
        str getEval() const override;
        const ntype& getType() const override;

    private:
        void _setOrigin(const node& newOrg);

    private:
        mgdType _type;
    };
}
