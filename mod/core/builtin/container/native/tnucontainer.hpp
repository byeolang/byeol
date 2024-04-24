#pragma once

#include "../tucontainable.hpp"

namespace namu {

    template <typename T, typename R>
    class tnucontainer : public instance, public tucontainable<T, R> {
        NAMU(ADT(tnucontainer, instance))
    };

    class node;
    typedef tnucontainer<node> nucontainer;
}
