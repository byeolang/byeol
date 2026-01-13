/// @file
#pragma once

#include "core/ast/func.hpp"

namespace by {
    /** @ingroup core
     *  @brief Nested function with captured scope
     *  @details Function that captures local scope at creation time.
     *  Clones function with shallow copied block scope for variable capture in @ref closure "closures".
     *  @note Created from @ref func with captured local variables, doesn't see variables defined after creation.
     */
    class _nout nestedFunc: public func {
        BY(ME(nestedFunc, func), CLONE(nestedFunc))

    public:
        explicit nestedFunc(const func& org, const scope& subs);

    protected:
        str _interactFrame(node& meObj, scope& s, nidx exN) override;
    };
}
