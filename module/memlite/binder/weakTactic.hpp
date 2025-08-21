/// @file
#pragma once

#include "../interface/instance.hpp"
#include "bindTacticable.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Weak reference binding tactic
    /// @details Implements weak reference semantics where bound instances
    /// can be freed independently of the binder. Does not affect instance lifetime.
    class _nout weakTactic: public bindTacticable {
        BY_ME(weakTactic, bindTacticable)

    public:
        void rel(binder& me) override;

        using super::bind;
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singleton;
    };
}
