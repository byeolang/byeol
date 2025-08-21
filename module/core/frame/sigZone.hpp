/// @file
#pragma once

#include "core/frame/signaler.hpp"

namespace by {
    /// @ingroup core
    /// @brief Signal handler zone management
    /// @details RAII wrapper for managing signal handler registration and cleanup.
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
