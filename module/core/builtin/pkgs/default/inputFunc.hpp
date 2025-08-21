/// @file
#pragma once

#include "core/ast/baseFunc.hpp"

namespace by {

    class _nout inputFunc: public baseFunc {
        BY(ME(inputFunc, baseFunc), CLONE(inputFunc))

    public:
        const ntype& getType() const override;

        const baseObj& getOrigin() const override;

        using super::run;
        str run(const args& a) override;
    };
}
