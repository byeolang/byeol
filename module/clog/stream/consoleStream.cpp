#include "clog/stream/consoleStream.hpp"

#include <iostream>

namespace by {

    BY_DEF_ME(consoleStream)

    me::consoleStream(): super(true) {}

    const std::string& me::getName() const {
        static std::string inner("consoleStream");
        return inner;
    }

    nbool me::logBypass(const nchar* msg) {
        if(super::logBypass(msg)) return true;

        platformAPI::log(msg);
        return false;
    }
}
