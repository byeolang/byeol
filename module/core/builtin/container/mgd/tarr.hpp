#pragma once

#include "core/builtin/container/mgd/arr.hpp"

namespace by {

    template <typename T, typename defaultElemType = T> class tarr: public arr {
        NM(ME(tarr, arr), INIT_META(tarr), CLONE(tarr))

    public:
        tarr(): super(defaultElemType().getOrigin()) {}

        tarr(const baseObj& elemType): super(elemType) {}

        tarr(const super& rhs): super(rhs) {}
    };
}
