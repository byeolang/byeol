#pragma once

#include "../interface/instance.hpp"
#include "bindTacticable.hpp"

namespace by {

    class _nout weakTactic: public bindTacticable {
        NM_ME(weakTactic, bindTacticable)

    public:
        void rel(binder& me) override;

        using super::bind;
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singleton;
    };
}
