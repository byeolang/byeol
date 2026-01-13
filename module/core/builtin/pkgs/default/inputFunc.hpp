/// @file
#pragma once

#include "core/ast/baseFunc.hpp"

namespace by {

    /** @ingroup core
     *  @brief Input function for reading user input
     *  @details Built-in function that reads input from standard input.
     */
    class _nout inputFunc: public baseFunc {
        BY(ME(inputFunc, baseFunc), CLONE(inputFunc))

    public:
        const ntype& getType() const override;

        const baseObj& getOrigin() const override;

        using super::eval;
        str eval(const args& a) override;
    };
}
