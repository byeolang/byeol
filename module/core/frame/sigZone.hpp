/// @file
#pragma once

#include "core/frame/signaler.hpp"

namespace by {
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
