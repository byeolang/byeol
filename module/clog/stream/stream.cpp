#include "clog/stream/stream.hpp"

namespace by {

    BY_DEF_ME(stream)

    me::stream(nbool isEnable): _isEnable(isEnable) {}

    me::~stream() {}

    nbool me::logBypass(const char* message) { return !message || !_isEnable; }

    nbool me::logBypass(const std::string& message) { return logBypass(message.c_str()); }

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
} // namespace by
