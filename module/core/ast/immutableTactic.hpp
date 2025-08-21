/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    class _nout immutableTactic: public strTactic {
        BY(ME(immutableTactic, strTactic))

    public:
        using super::bind;
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singleton;
    };
}
