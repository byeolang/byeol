#pragma once

#include "core/ast/baseFunc.hpp"

namespace nm {

    class _nout inputFunc: public baseFunc {
        NM(ME(inputFunc, baseFunc), CLONE(inputFunc))

    public:
        const ntype& getType() const override;

        const baseObj& getOrigin() const override;

        using super::run;
        str run(const args& a) override;
    };
}
