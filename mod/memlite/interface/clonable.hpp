#pragma once

#include "../common.hpp"

namespace nm {

    class _nout clonable {
        NM_ME(clonable)
        NM_INIT_META(me)

    public:
        virtual ~clonable() {}

        virtual me* clone() const = 0;
        virtual me* cloneDeep() const { return clone(); }
    };
}
