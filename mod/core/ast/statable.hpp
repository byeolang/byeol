#pragma once

#include "../common.hpp"

namespace nm {
    enum state {
        RELEASED = 0,
        PARSED = 1,
        VERIFIED,
        LINKED,
    };

    class _nout statable {
        NM(ME(statable))

    public:
        virtual state getState() const = 0;
        virtual void setState(state new1) = 0;
    };
}
