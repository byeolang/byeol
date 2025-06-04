#pragma once

#include "core/common.hpp"

namespace nm {
    enum state {
        RELEASED = 0,
        PARSED = 1, // all AST has been parsed and expanded.
        VERIFIED,   // verifier checked this node.
        LINKED,     // run and attached to system.
    };

    class _nout statable {
        NM(ME(statable))

    public:
        virtual state getState() const = 0;
        virtual void setState(state new1) = 0;
    };
}
