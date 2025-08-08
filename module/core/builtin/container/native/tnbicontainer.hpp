#pragma once

#include "core/builtin/container/tbicontainable.hpp"

namespace by {

    template <typename K, typename V>
    class tnbicontainer: public instance, public tbicontainable<K, V> {
        NM(ADT(tnbicontainer, instance))
    };

    typedef tnbicontainer<std::string, node> nbicontainer;
}
