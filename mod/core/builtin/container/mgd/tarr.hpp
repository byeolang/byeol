#pragma once

#include "arr.hpp"

namespace nm {

    template <typename T, typename defaultElemType = T> class tarr: public arr {
        NM(ME(tarr, arr), INIT_META(tarr), CLONE(tarr))

    public:
        tarr(): super(defaultElemType().getOrigin()) {}

        tarr(const baseObj& elemType): super(elemType) {}

        tarr(const super& rhs): super(rhs) {}
    };
}
