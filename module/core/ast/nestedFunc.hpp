#pragma once

#include "core/ast/func.hpp"

namespace by {
    /// nested func:
    ///     when you eval nested func, actually it will clone a new func with the current
    ///     shallow copied block scope. so it can captures local scope at that time.
    ///
    ///     it doesn't know about local variables after you define it.
    ///     it contains copied block scope. and will inject just before runs block stmt.
    class _nout nestedFunc: public func {
        NM(ME(nestedFunc, func), CLONE(nestedFunc))

    public:
        explicit nestedFunc(const func& org, const scope& subs);

    protected:
        str _interactFrame(node& meObj, scope& s, nidx exN) override;
    };
}
