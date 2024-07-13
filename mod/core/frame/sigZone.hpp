#pragma once

#include "signaler.hpp"

namespace nm {
    class _nout sigZone {
        NM(ME(sigZone))

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
