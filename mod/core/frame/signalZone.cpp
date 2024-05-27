#include "signalZone.hpp"

namespace namu {
    NAMU(DEF_ME(signalZone))

    me::signalZone(sigHandler handler): _handler(handler) {
        signaler::get().addSignal(_handler);
    }
    me::~signalZone() {
        signaler::get().delSignal(_handler);
    }
}
