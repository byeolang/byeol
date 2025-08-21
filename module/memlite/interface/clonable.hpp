/// @file
#pragma once

#include "memlite/common.hpp"

namespace by {

    class _nout clonable {
        BY_ME(clonable)
        BY_INIT_META(me)

    public:
        virtual ~clonable() {}

        virtual me* clone() const = 0;

        virtual me* cloneDeep() const {
            me* ret = clone();
            ret->onCloneDeep(*this);
            return ret;
        }

        virtual void onCloneDeep(const me& from) {}
    };
}
