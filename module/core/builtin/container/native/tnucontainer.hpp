#pragma once

#include "core/builtin/container/tucontainable.hpp"

namespace by {

    template <typename T, typename R = T*, typename RSquare = T&>
    class tnucontainer: public instance, public tucontainable<T, R, RSquare> {
        NM(ADT(tnucontainer, instance))
    };

    class node;
    typedef tnucontainer<node> nucontainer;
}
