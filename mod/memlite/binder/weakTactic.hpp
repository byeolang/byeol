#pragma once

#include "bindTacticable.hpp"
#include "../interface/instance.hpp"

namespace namu {

    class _nout weakTactic : public bindTacticable {
        NAMU_ME(weakTactic, bindTacticable);

    public:
        void rel(binder& me) override;
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singletone;
    };
}
