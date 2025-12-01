/// @file
#pragma once

#include "core/common.hpp"

namespace by {
    enum state {
        RELEASED = 0,
        PARSED = 1, // all AST has been parsed and expanded.
        VERIFIED,   // verifier checked this node.
        LINKED,     // eval and attached to system.
    };

    /// @ingroup core
    /// @brief Interface for objects with compilation state
    /// @details Provides state management for compilation phases (parsed, verified, linked).
    class _nout statable {
        BY(ME(statable))

    public:
        virtual state getState() const = 0;
        virtual void setState(state new1) = 0;
    };
}
