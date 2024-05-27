#pragma once

#include "signaler.hpp"

namespace namu {
    class _nout signalZone {
        NAMU(ME(signalZone))

    public:
        signalZone(sigHandler handler);
        ~signalZone();

    private:
        sigHandler _handler;
    };
}
