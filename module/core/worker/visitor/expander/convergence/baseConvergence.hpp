/// @file
#pragma once

#include "core/common.hpp"

namespace by {
    /// @ingroup core
    /// @brief Base class for convergence algorithms
    /// @details Abstract interface for convergence operations in the expander visitor pattern.
    class _nout baseConvergence: public instance {
    public:
        virtual nbool converge() = 0;
        virtual nbool convergeWithoutFrame() = 0;
    };
}
