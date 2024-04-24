#include "consoleStream.hpp"
#include <iostream>

namespace wrd {

    WRD_DEF_ME(consoleStream)

    const char* me::getName() const { return "consoleStream"; }

    wbool me::dump(const char* message) {
        using std::cout;
        if(super::dump(message)) return true;

        cout << message;
        return false;
    }
}
