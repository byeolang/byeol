/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    class _nout validable {
        BY_ME(validable)
        BY_INIT_META(me)

    public:
        virtual nbool isValid() const = 0;
    };
}
