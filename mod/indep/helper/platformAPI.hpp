#pragma once

#include "buildFeature.hpp"
#include <vector>

namespace namu {
    enum consoleColor {
        CONSOLE_COLOR_START = 0,
        BLACK=CONSOLE_COLOR_START, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
        LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
        LIGHTMAGENTA, YELLOW,
        CONSOLE_COLOR_END,
        WHITE = CONSOLE_COLOR_END
    };

    namespace platformAPI {
        const std::string& foreColor(consoleColor fore);
        const std::string& backColor(consoleColor back);
        std::string createNowTime(const std::string& strftime_format);
        std::vector<std::string> callstack();
        std::string demangle(const nchar* org);
        std::string filterDemangle(const nchar* org);
        std::string getExecPath();
        std::string exec(const std::string& cmd);
        nulong getNowMs();
        std::string toAddrId(void* inst);
    };
}
