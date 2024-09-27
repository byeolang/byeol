#pragma once

#include "../../../ast/baseFunc.hpp"

namespace nm {

    class _nout inputFunc: public baseFunc {
        NM(ME(inputFunc, baseFunc), CLONE(inputFunc))

    public:
        const ntype& getType() const override;

        const baseObj& getOrigin() const override;

        str run(const args& a) override;
    };
}
