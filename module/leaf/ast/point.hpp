#pragma once

#include "leaf/common.hpp"

namespace by {
    struct _nout point {
        point& operator++();

    public:
        nbool isOrigin() const;
        void rel();

    public:
        ncnt row;
        ncnt col;
    };

    struct _nout area {
        area& operator++();

    public:
        void rel();

    public:
        point start;
        point end;
    };
}
