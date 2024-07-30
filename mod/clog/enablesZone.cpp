#include "enablesZone.hpp"

#include "logger.hpp"

namespace nm {
    NM(DEF_ME(enablesZone))

    me::enablesZone() { _enables = logger::get().getEnables(); }

    me::enablesZone(nbool newVal) {
        _enables = logger::get().getEnables();
        setEnable(newVal);
    }

    me::~enablesZone() { logger::get().setEnables(_enables); }

    const enables& me::getPrev() const { return _enables; }

    me& me::setPrev() {
        logger::get().setEnables(getPrev());
        return *this;
    }

    me& me::rel() {
        _enables.clear();
        return *this;
    }

    me& me::setEnable(nbool newVal) {
        logger::get().setEnable(newVal);
        return *this;
    }

    me& me::setEnable(const std::string& streamName, nbool newVal) {
        auto& stream = logger::get().getStream(streamName.c_str());
        if(!nul(stream)) stream.setEnable(newVal);
        return *this;
    }
} // namespace nm
