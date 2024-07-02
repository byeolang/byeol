#pragma once

#include "../tucontainable.hpp"
#include "../../../type/exceptional.hpp"

namespace nm {

    template <typename T, typename R>
    class tnucontainer : public instance, public tucontainable<T, R>, public exceptional {
        NAMU(ADT(tnucontainer, instance))
    };

    class node;
    typedef tnucontainer<node> nucontainer;
}
