#pragma once

#include "dep.hpp"

namespace nm {
    class arithmeticObj;
    class obj;
    class nInt;
    struct visitInfo;
    class node;
    typedef tstr<node> str;

    std::string convert(const nint& it);
    std::string convert(const nflt& it);
    std::string convert(const nchar& it);
    std::string convert(const nbool& it);
    std::string convert(const std::string& it);
    std::string convert(const nchar* it);
    std::string convert(const ndbl& it);

    std::string convert(const typeProvidable& it);
    std::string convert(const type& it);

    std::string convert(const arithmeticObj& it);
    std::string convert(const visitInfo& it);
    std::string convert(const str& it);

    template <typename... Ts>
    void richLog(logLv::level lv, const nchar* tag, const nchar* filename, const nchar* func, nint line, const nchar* fmt, const Ts&... args) {
        ::nm::logger::get().log(lv, tag, filename, func, line, fmt, convert((const Ts&) args).c_str()...);
    }

#define NM_RE(fmt, ...) ::nm::richLog(nm::logLv::ERR, NM_TAG, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NM_RW(fmt, ...) ::nm::richLog(nm::logLv::WARN, NM_TAG, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NM_RI(fmt, ...) ::nm::richLog(nm::logLv::INFO, NM_TAG, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NM_EX(code, ...) this->ex(code, __VA_ARGS__)
#ifdef NM_DEBUG
#   define NM_RDE(fmt, ...) NM_E(fmt, ## __VA_ARGS__)
#   define NM_RDW(fmt, ...) NM_W(fmt, ## __VA_ARGS__)
#   define NM_RDI(fmt, ...) NM_I(fmt, ## __VA_ARGS__)
#else
#   define NM_RDE(fmt, ...) void()
#   define NM_RDW(fmt, ...) void()
#   define NM_RDI(fmt, ...) void()
#endif
}
