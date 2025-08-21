/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    /// @ingroup core
    /// @brief Interface for objects with validation state
    /// @details Provides validation checking for objects that can be in valid/invalid states.
    class _nout validable {
        BY_ME(validable)
        BY_INIT_META(me)

    public:
        virtual nbool isValid() const = 0;
    };
}
