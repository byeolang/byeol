#pragma once

#include "../common.hpp"

namespace nm {
    struct point;
    class _nout exceptional {
        NAMU(ME(exceptional))

    public:
        void ex(nint code, ...);
        void ex(const point& src, int code, ...);
    };
}
