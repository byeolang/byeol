#pragma once

#include "signaler.hpp"

namespace namu {
    class _nout sigZone {
        NAMU(ME(sigZone))

    public:
        sigZone(sigHandler handler);
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
