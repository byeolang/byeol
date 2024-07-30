#pragma once

#include "../interface/instance.hpp"
#include "bindTacticable.hpp"

namespace nm {

    class _nout weakTactic: public bindTacticable {
        NM_ME(weakTactic, bindTacticable)

    public:
        void rel(binder& me) override;
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singletone;
    };
}
