#pragma once

#include "../common.hpp"

namespace namu {

    class _nout clonable {
        NAMU_ME(clonable)
        NAMU_INIT_META(me)

    public:
        virtual ~clonable() {}

        virtual me* clone() const = 0;
        virtual me* cloneDeep() const { return clone(); }
    };
}
