#pragma once

#include "buildFeature.hpp"
#include <vector>

namespace nm {
    enum consoleColor {
        CONSOLE_COLOR_START = 0,
        BLACK=CONSOLE_COLOR_START, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
        LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
        LIGHTMAGENTA, YELLOW,
        CONSOLE_COLOR_END,
        WHITE = CONSOLE_COLOR_END
    };

    namespace platformAPI {
        _nout const std::string& foreColor(consoleColor fore);
        _nout const std::string& backColor(consoleColor back);
        _nout std::string createNowTime(const std::string& strftime_format);
        _nout std::vector<std::string> callstack();
        _nout std::string demangle(const nchar* org);
        _nout std::string filterDemangle(const nchar* org);
        _nout std::string getExecPath();
        _nout std::string exec(const std::string& cmd);
        _nout nulong getNowMs();
        _nout std::string toAddrId(const void* inst);
    };
}
