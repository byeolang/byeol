#include "clog/stream/stream.hpp"

namespace nm {

    NM_DEF_ME(stream)

    me::stream(nbool isEnable): _isEnable(isEnable) {}

    me::~stream() {}

    nbool me::logBypass(const char* message) { return !_isEnable; }

    nbool me::isNull() const { return !this; }

    nbool me::isEnable() const { return _isEnable; }

    void me::setEnable(nbool isEnable) { _isEnable = isEnable; }

    nbool me::init() {
        _isEnable = true;
        return !isInit();
    }

    nbool me::isInit() const { return _isEnable; }

    nbool me::rel() {
        _isEnable = false;
        return false;
    }
} // namespace nm
