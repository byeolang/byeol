#include "consoleStream.hpp"

#include <iostream>

namespace nm {

    NM_DEF_ME(consoleStream)

    me::consoleStream(): super(true) {}

    const std::string& me::getName() const {
        static std::string inner("consoleStream");
        return inner;
    }

    nbool me::logBypass(const char* msg) {
        if(super::logBypass(msg)) return true;

        platformAPI::log(msg);
        return false;
    }
}
