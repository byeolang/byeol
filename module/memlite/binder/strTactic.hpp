/// @file
#pragma once

#include "memlite/binder/weakTactic.hpp"

namespace by {

    /** @ingroup memlite
     *  @brief Strong reference binding tactic
     *  @details Implements strong reference semantics where bound instances
     *  are kept alive as long as the binder exists. Extends weak tactic with reference counting.
     */
    class _nout strTactic: public weakTactic {
        BY_ME(strTactic, weakTactic)

    public:
        void rel(binder& me) override;

        using super::bind;
        nbool bind(binder& me, const instance& it) override;

        static me singleton;
    };
}
