#pragma once

#include "core/common.hpp"

namespace nm {

    class _nout immutableTactic: public strTactic {
        NM(ME(immutableTactic, strTactic))

    public:
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singleton;
    };
}
