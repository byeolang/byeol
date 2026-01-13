/// @file
#pragma once

#include <vector>

#include "indep/helper/buildFeature.hpp"

namespace by {
    enum consoleColor {
        CONSOLE_COLOR_START = 0,
        BLACK = CONSOLE_COLOR_START,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        CONSOLE_COLOR_END,
        WHITE = CONSOLE_COLOR_END
    };

    /** @ingroup indep
     *  @brief Platform-independent API wrapper for OS-specific operations
     *  @details Provides a unified interface for platform-dependent operations, acting as a
     *  buffer layer between platform-specific APIs and the rest of the codebase.
     *
     *  For example, colored text output requires different approaches on different platforms:
     *  POSIX systems use ANSI escape sequences, while Windows uses WINAPI. Using
     *  platformAPI::foreColor() allows writing platform-independent code.
     *
     *  @section Usage
     *  Example of platform-independent colored console output:
     *  @code
     *      cout << foreColor(LIGHTGRAY) << "("
     *           << foreColor(YELLOW) << _encodeNewLine(rightName)
     *           << foreColor(LIGHTGRAY) << ")";
     *  @endcode
     *
     *  Other common operations include:
     *  - Demangling C++ names with demangle()
     *  - Getting call stacks with callstack()
     *  - Cross-platform command execution with exec()
     *  - Core dump limit management with unlimitCoreDump()
     */
    namespace platformAPI {
        const _nout std::string& foreColor(consoleColor fore);
        const _nout std::string& backColor(consoleColor back);
        _nout std::string createNowTime(const std::string& strftime_format);
        _nout std::vector<std::string> callstack();
        _nout std::string demangle(const nchar* org);
        _nout std::string filterDemangle(const nchar* org);
        _nout std::string getExecPath();
        _nout std::string exec(const std::string& cmd);
        _nout nuint64 getNowMs();
        _nout std::string toAddrId(const void* inst);
        _nout void log(const std::string& msg);
        _nout void log(const std::string* it);
        _nout std::string format(const nchar* fmt, ...);
        _nout std::string format(const nchar* fmt, va_list args);
        _nout std::string format(const std::string& fmt, va_list args);
        _nout std::string format(const std::string* fmt, va_list args);
        _nout void crash(const nchar* msg, ...);
        _nout void crash(const std::string& msg, va_list args);
        _nout void crash(const std::string* msg, va_list args);
        _nout void crash(const std::string& msg);
        _nout void crash(const std::string* msg);
        _nout nbool unlimitCoreDump();
    };
} // namespace by
