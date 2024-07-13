#include "sigZone.hpp"

namespace nm {
    NM(DEF_ME(sigZone))

    me::sigZone(const sigHandler& handler): _handler(handler) {
        prepare(handler).setHandler();
    }
    me::sigZone() {}

    me::~sigZone() {
        relHandler();
    }

    me& me::prepare(sigHandler handler) {
        _handler = std::move(handler);
        return *this;
    }

    me& me::setHandler() {
        signaler::get().addSignal(_handler);
        return *this;
    }

    void me::relHandler() {
        signaler::get().delSignal(_handler);
    }
}
