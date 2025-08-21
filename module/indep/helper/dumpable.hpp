/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/def.hpp"

namespace by {
    /// @ingroup indep
    /// @brief Interface for objects that can dump their state
    /// @details Provides a standard interface for objects to output
    /// their internal state for debugging or diagnostic purposes.
    class _nout dumpable {
    public:
        /// @brief Dump object state for debugging
        virtual void dump() const = 0;
    };
}
