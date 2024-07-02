#include "consoleStream.hpp"
#include <iostream>

namespace nm {

    NAMU_DEF_ME(consoleStream)

    me::consoleStream(): super(true) {}

    const std::string& me::getName() const {
        static std::string inner("consoleStream");
        return inner;
    }

    nbool me::logBypass(const char* message) {
        using std::clog;
        if(super::logBypass(message)) return true;

        clog << message;
        return false;
    }
}
