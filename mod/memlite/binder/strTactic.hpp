#pragma once

#include "weakTactic.hpp"

namespace nm {

    class _nout strTactic : public weakTactic {
        NAMU_ME(strTactic, weakTactic);

    public:
        void rel(binder& me) override;
        nbool bind(binder& me, const instance& it) override;

        static me singletone;
    };
}
