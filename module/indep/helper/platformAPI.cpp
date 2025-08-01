#include "indep/helper/platformAPI.hpp"

#include "indep/helper/typeTrait.hpp"
#include <cstdarg>
#if BY_BUILD_PLATFORM == BY_TYPE_WINDOWS
#    include <windows.h>

#    include <sstream>
#elif BY_BUILD_PLATFORM == BY_TYPE_LINUX || BY_BUILD_PLATFORM == BY_TYPE_MACOS
#    include <cxxabi.h>
#    include <dlfcn.h> // for dladdr()
#    include <execinfo.h>
#    include <sys/time.h>
#    include <unistd.h>

#    include <algorithm>
#    include <iostream>
#    include <regex>
#    include <string>
#    include <vector>
#endif
#if BY_BUILD_PLATFORM == BY_TYPE_MACOS
#    include <mach-o/dyld.h>
#endif
#include <sstream>
#include <time.h>

namespace by {

    namespace platformAPI {
        constexpr nint PATH_MAX_LEN = 256;
        using namespace std;

#if defined(BY_BUILD_PLATFORM_IS_LINUX) || defined(BY_BUILD_PLATFORM_IS_MAC)
        namespace {
            bool _isAnsiColorTerminal() {
                static vector<const nchar*> samples = {"xterm", "rxvt", "vt100", "linux", "screen", "tmux"};
                if(!getenv("TERM")) return false;
                string var = getenv("TERM");

                return find_if(samples.begin(), samples.end(),
                           [&](const string& e) { return var.find(e) != string::npos; }) != samples.end();
            }
        }
#endif

        const string& foreColor(consoleColor fore) {
#ifdef __EMSCRIPTEN__
            // TODO: Unreachable code exception raised if we don't handle it.
            //       find solution not to use this ifdef __EMSCRIPTEN__ block.
            static string inner = "";
            return inner;
#elif BY_BUILD_PLATFORM == BY_TYPE_WINDOWS
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK << 4 | fore);
            static string inner;
            return inner;
#elif BY_BUILD_PLATFORM == BY_TYPE_LINUX || BY_BUILD_PLATFORM == BY_TYPE_MACOS
            static bool is_terminal_supporting = _isAnsiColorTerminal();
            if(!is_terminal_supporting) {
                static string inner;
                return inner;
            }

            static vector<string> fores = {"\x1B[0;30m", "\x1B[0;34m", "\x1B[0;32m",
                "\x1B[0;36m", // black, blue, green, cyan
                "\x1B[0;31m", "\x1B[0;35m", "\x1B[0;33m",
                "\x1B[0;37m", // red, purple, yellow, white
                "\x1B[1;30m", "\x1B[1;34m", "\x1B[1;32m",
                "\x1B[1;36m", // same ones but more lighter than above.
                "\x1B[1;31m", "\x1B[1;35m", "\x1B[1;33m", "\x1B[1;37m"};
            return fores[fore];
#endif
        }

        const string& backColor(consoleColor back) {
            static string inner;
#if defined(__EMSCRIPTEN__)
            return inner;
#elif BY_BUILD_PLATFORM == BY_TYPE_WINDOWS
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), back << 4 | WHITE);
            return inner;

#elif BY_BUILD_PLATFORM == BY_TYPE_LINUX || BY_BUILD_PLATFORM == BY_TYPE_MACOS
            static bool is_terminal_supporting = _isAnsiColorTerminal();
            if(!is_terminal_supporting) return inner;

            static vector<string> backs = {
                "\x1B[0;40m", "\x1B[0;44m", "\x1B[0;42m", "\x1B[0;46m", // black, blue, green, cyan
                "\x1B[0;41m", "\x1B[0;45m", "\x1B[0;43m",
                "\x1B[0;47m",                                           // red, purple, yellow, white
                "\x1B[1;40m", "\x1B[1;44m", "\x1B[1;42m", "\x1B[1;46m", // black, blue, green, cyan
                "\x1B[1;41m", "\x1B[1;45m", "\x1B[1;43m",
                "\x1B[1;47m", // red, purple, yellow, white
            };
            return backs[back];
#endif
        }

        string createNowTime(const string& strftime_format) {
            constexpr ncnt TIME_BUF_LEN = 80;
            time_t t = time(0);
            struct tm* timeinfo = localtime(&t);

            char buffer[TIME_BUF_LEN];
            strftime(buffer, TIME_BUF_LEN, strftime_format.c_str(), timeinfo);

            return buffer;
        }

        nuint64 getNowMs() {
#if BY_BUILD_PLATFORM == BY_TYPE_LINUX || BY_BUILD_PLATFORM == BY_TYPE_MACOS
            constexpr ncnt MILLI_PER_USEC = 1000;

            struct timeval tval {};

            gettimeofday(&tval, NULL);
            return tval.tv_usec / MILLI_PER_USEC;
#else
            return 0L;
#endif
        }

        string toAddrId(const void* inst) {
            stringstream ss;
            ss << inst;
            string raw = ss.str();
            return raw.substr(raw.length() - 4);
        }

        void log(const std::string& msg) {
#ifndef __EMSCRIPTEN__
            std::clog << msg;
#else
            std::cout << msg;
#endif
        }

        void log(const std::string* it) BY_SIDE_FUNC(log);

        string getExecPath() {
#if BY_BUILD_PLATFORM == BY_TYPE_LINUX
            nchar res[PATH_MAX_LEN];
            nuint count = readlink("/proc/self/exe", res, PATH_MAX_LEN);

            return string(res, (count > 0) ? count : 0);
#elif BY_BUILD_PLATFORM == BY_TYPE_MACOS
            nchar res[PATH_MAX_LEN];
            nuint size = PATH_MAX_LEN + 1;

            _NSGetExecutablePath(res, &size);
            return string(res, size);
#else
            return string();
#endif
        }

        string exec(const string& cmd) {
#if BY_BUILD_PLATFORM == BY_TYPE_LINUX || BY_BUILD_PLATFORM == BY_TYPE_MACOS
            constexpr ncnt EXE_BUF_LEN = 128;
            nchar buf[EXE_BUF_LEN] = {
                0,
            };
            string res;
            shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

            while(!feof(pipe.get()))
                if(fgets(buf, EXE_BUF_LEN, pipe.get()) != nullptr) res += buf;

            return res;
#else
            return "";
#endif
        }

        vector<string> callstack() {
            vector<string> ret;
#if BY_BUILD_PLATFORM == BY_TYPE_LINUX || BY_BUILD_PLATFORM == BY_TYPE_MACOS

            constexpr int BT_SIZE = 100;
            void* rawCallstacks[BT_SIZE] = {
                nullptr,
            };

            int len = backtrace(rawCallstacks, BT_SIZE);
            char** callstacks = backtrace_symbols(rawCallstacks,
                len); // don't use OR, you can't free with dereference
            if(!callstacks) return ret;

            for(int n = 0; n < len; n++) {
                Dl_info info;
                if(dladdr(rawCallstacks[n], &info)) {
                    auto demangled = demangle(info.dli_sname);
                    if(demangled[0] == '?' || demangled[0] == '\0') continue;

                    ret.push_back(demangled.substr(0, demangled.size()));
                }
            }

            free(callstacks);
#endif
            return ret;
        }

        string demangle(const nchar* org) {
#if !defined(BY_BUILD_PLATFORM_IS_WINDOWS) && !defined(__EMSCRIPTEN__)
            nchar* demangled = nullptr;
            int status = 0;
            if(!org) org = "";

            demangled = ::abi::__cxa_demangle(org, 0, 0, &status);
            string ret(status == 0 ? demangled : org);
            free(demangled);
            return ret;
#endif
            return string(org);
        }

        string filterDemangle(const nchar* org) {
#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
            string raw(org);
            auto n = raw.rfind(" ");
#else
            const string& raw = demangle(org);
            auto n = raw.find("::");
#endif
            return raw.size() > 3 ? raw.substr(n + 2) : org;
        }

        std::string format(const nchar* fmt, ...) {
            va_list args;
            va_start(args, fmt);

            auto ret = format(std::string(fmt), args);
            va_end(args);
            return ret;
        }

        constexpr nint MAX_BUF = 512;

        std::string format(const std::string& fmt, va_list args) { return format(fmt.c_str(), args); }

        std::string format(const std::string* fmt, va_list args) BY_SIDE_FUNC(fmt, format(*fmt, args), std::string());

        std::string format(const nchar* fmt, va_list args) {
            nchar buf[MAX_BUF] = {
                0,
            };
            vsnprintf(buf, MAX_BUF, fmt, args);

            return buf;
        }

        void crash(const nchar* msg, ...) {
            va_list args;
            va_start(args, msg);

            crash(std::string(msg), args);

            va_end(args);
        }

        void crash(const std::string& msg, va_list args) { crash(format(msg, args)); }

        void crash(const std::string* msg, va_list args) BY_SIDE_FUNC(msg, crash(*msg, args), void());

        void crash(const std::string& msg) {
            log(" * * * Interpreter CRASH * * *\n");
            log(" reason: " + msg + "\n");
            log(" callstack:\n");
            callstack();
            log(" * * * * * * * * * * * * * * *\n");
            abort();
        }

        void crash(const std::string* it) BY_SIDE_FUNC(crash);
    } // namespace platformAPI
} // namespace by
