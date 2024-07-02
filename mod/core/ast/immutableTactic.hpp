#pragma once

#include "../common.hpp"

namespace nm {

    class _nout immutableTactic : public strTactic {
        NAMU(ME(immutableTactic, strTactic))

    public:
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singletone;
    };
}
