/// @file
#pragma once

#include "memlite/common.hpp"

namespace by {

    class binder;
    class instance;

    /// @ingroup memlite
    /// @brief Interface for binding tactics in memory management
    /// @details Defines the contract for different binding strategies used by binder objects.
    /// Allows customization of binding and release behaviors.
    class _nout bindTacticable {
    public:
        virtual void rel(binder& me) = 0;
        virtual nbool bind(binder& me, const instance& it) = 0;
        nbool bind(binder& me, const instance* it) BY_SIDE_FUNC(it, bind(me, *it), false)
    };
}
