#pragma once

#include "core/common.hpp"

namespace by {
    class _nout baseConvergence: public instance {
    public:
        virtual nbool converge() = 0;
        virtual nbool convergeWithoutFrame() = 0;
    };
}
