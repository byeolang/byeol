#pragma once

#include "core/builtin/container/tucontainable.hpp"

namespace nm {

    template <typename T, typename R>
    class tnucontainer: public instance, public tucontainable<T, R> {
        NM(ADT(tnucontainer, instance))
    };

    class node;
    typedef tnucontainer<node> nucontainer;
}
