#pragma once

#include "memlite/binder/weakTactic.hpp"

namespace by {

    class _nout strTactic: public weakTactic {
        NM_ME(strTactic, weakTactic)

    public:
        void rel(binder& me) override;

        using super::bind;
        nbool bind(binder& me, const instance& it) override;

        static me singleton;
    };
}
