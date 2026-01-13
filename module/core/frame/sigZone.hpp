/// @file
#pragma once

#include "core/frame/signaler.hpp"

namespace by {
    /** @ingroup core
     *  @brief Signal handler zone management
     *  @details RAII wrapper for @ref signaler that manages signal handler registration and cleanup. When a sigZone
     *  instance is defined within a block scope, it uses signaler to execute a lambda function if a signal arrives
     *  during code execution within that block.
     */
    class _nout sigZone {
        BY(ME(sigZone))

    public:
        sigZone(const sigHandler& handler);
        sigZone();
        ~sigZone();

    public:
        me& prepare(sigHandler handler);
        me& setHandler();
        void relHandler();

    private:
        sigHandler _handler;
    };
}
