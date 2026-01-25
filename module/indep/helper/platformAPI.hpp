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
        /**
         * @brief Returns platform-specific string for console foreground color
         * @return On POSIX returns ANSI escape sequence, on Windows modifies console and
         *         returns empty string. Returns empty if terminal doesn't support colors.
         */
        const _nout std::string& foreColor(consoleColor fore);

        /**
         * @brief Returns platform-specific string for console background color
         * @return On POSIX returns ANSI escape sequence, on Windows modifies console and
         *         returns empty string. Returns empty if terminal doesn't support colors.
         */
        const _nout std::string& backColor(consoleColor back);

        /**
         * @brief Creates formatted timestamp using strftime format
         * @return Formatted time string representing current local time
         */
        _nout std::string createNowTime(const std::string& strftime_format);

        /**
         * @brief Captures current call stack for debugging
         * @return Vector of demangled function names, excluding invalid entries.
         *         Returns empty if execinfo unavailable.
         * @note Requires execinfo.h support (BY_HAS_EXECINFO == 1)
         */
        _nout std::vector<std::string> callstack();

        /**
         * @brief Demangles C++ symbol name to human-readable form
         * @return Demangled name on POSIX, original string on Windows/WASM or if demangling fails
         */
        _nout std::string demangle(const nchar* org);

        /**
         * @brief Demangles C++ symbol and extracts only the function name portion
         * @return Function name after "::" on POSIX, after last space on Windows
         */
        _nout std::string filterDemangle(const nchar* org);

        /**
         * @brief Retrieves absolute path of currently running executable
         * @return Absolute path on Linux/macOS, empty string on Windows or error
         * @note Uses /proc/self/exe on Linux, _NSGetExecutablePath on macOS
         */
        _nout std::string getExecPath();

        /**
         * @brief Executes shell command and captures output
         * @return Complete stdout output. Returns empty on Windows or execution failure.
         * @note POSIX only. Uses popen/pclose internally.
         */
        _nout std::string exec(const std::string& cmd);

        _nout nuint64 getNowMs();

        /**
         * @brief Converts pointer to 4-character hexadecimal identifier
         * @return Last 4 hex digits of address
         * @note Commonly used in logging with "@" prefix (e.g., "obj@a4f2")
         */
        _nout std::string toAddrId(const void* inst);

        _nout void log(const std::string& msg);
        _nout void log(const std::string* it);

        /**
         * @brief Formats string using printf-style specifiers
         * @return Formatted string (max 512 characters)
         */
        _nout std::string format(const nchar* fmt, ...);

        _nout std::string format(const nchar* fmt, va_list args);
        _nout std::string format(const std::string& fmt, va_list args);
        _nout std::string format(const std::string* fmt, va_list args);

        /**
         * @brief Terminates program with error message and call stack dump
         * @note Outputs crash banner, reason, callstack, then calls abort()
         */
        _nout void crash(const nchar* msg, ...);

        _nout void crash(const std::string& msg, va_list args);
        _nout void crash(const std::string* msg, va_list args);
        _nout void crash(const std::string& msg);
        _nout void crash(const std::string* msg);

        /**
         * @brief Enables unlimited core dump size for debugging
         * @return true if successfully set, false otherwise
         * @note On POSIX sets RLIMIT_CORE to infinity, on Windows registers minidump filter.
         *       Should be called early in debug builds.
         */
        _nout nbool unlimitCoreDump();
    };
} // namespace by
