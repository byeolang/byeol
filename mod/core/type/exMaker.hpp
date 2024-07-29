#pragma once

#include "../common.hpp"

namespace nm {
    struct point;
    struct _nout exMaker {
        NM(ME(exMaker))

    public:
        static void make(nint code, ...);
        static void make(const point& src, int code, ...);
    };

#define ex(...) nothing(), ::nm::exMaker::make(__VA_ARGS__)
}
