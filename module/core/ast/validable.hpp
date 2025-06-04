#pragma once

#include "core/common.hpp"

namespace nm {

    class _nout validable {
        NM_ME(validable)
        NM_INIT_META(me)

    public:
        virtual nbool isValid() const = 0;
    };
}
