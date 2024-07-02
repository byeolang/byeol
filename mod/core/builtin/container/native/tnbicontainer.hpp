#pragma once

#include "../tbicontainable.hpp"
#include "../../../type/exceptional.hpp"

namespace nm {

    template <typename K, typename V>
    class tnbicontainer : public instance, public tbicontainable<K, V>, public exceptional {
        NAMU(ADT(tnbicontainer, instance))
    };

    typedef tnbicontainer<std::string, node> nbicontainer;
}
