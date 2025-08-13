#pragma once

#include "clog/logger.hpp"

namespace by {

    template <typename T> struct noWrap {
        noWrap(T rhs): data(rhs) {}

        T unwrap() const { return data; }

        T data;
    };

    template <typename T> struct useWrap {
        static_assert(true, "not supported wrapper for richLog");
    };

    struct _nout strWrap {
        strWrap(const std::string& rhs);
        const nchar* unwrap() const;
        std::string data;
    };

    template <typename T, bool Signed, int Size>
    using tifIntType = typename std::enable_if<std::is_integral_v<T> == true && std::is_signed<T>{} == Signed &&
        sizeof(T) * 8 == Size>::type;

    template <typename T> noWrap<T> __convert__(T rhs, tifIntType<T, true, 32>* = nullptr) { return rhs; }

    template <typename T> noWrap<T> __convert__(T rhs, tifIntType<T, true, 64>* = nullptr) { return rhs; }

    template <typename T> noWrap<T> __convert__(T rhs, tifIntType<T, false, 32>* = nullptr) { return rhs; }

    template <typename T> noWrap<T> __convert__(T rhs, tifIntType<T, false, 64>* = nullptr) { return rhs; }

    _nout noWrap<nflt> __convert__(nflt rhs);
    _nout noWrap<nflt> __convert__(nflt* rhs);
    _nout noWrap<nchar> __convert__(nchar rhs);
    _nout noWrap<nchar> __convert__(nchar rhs);
    _nout strWrap __convert__(nbool rhs);
    _nout strWrap __convert__(nbool rhs);
    _nout noWrap<const nchar*> __convert__(const nchar* rhs);
    _nout noWrap<ndbl> __convert__(ndbl rhs);
    _nout noWrap<ndbl> __convert__(ndbl rhs);
    _nout noWrap<nshort> __convert__(nshort rhs);
    _nout noWrap<nshort> __convert__(nshort rhs);
    _nout noWrap<nushort> __convert__(nushort rhs);
    _nout noWrap<nushort> __convert__(nushort rhs);
    _nout strWrap __convert__(const std::string* rhs);
    _nout strWrap __convert__(const std::string& rhs);
    _nout strWrap __convert__(void* rhs);

    template <typename... Ts>
    void richLog(errLv::level lv, const std::string& filename, const nchar* func, nint line, const nchar* fmt,
        const Ts&... args) {
        ::by::logger::get().log(lv, filename, func, line, fmt, __convert__((const Ts&) args).unwrap()...);
    }

    template <typename... Ts>
    void dbgRichLog(errLv::level lv, const std::string& filename, const nchar* func, nint line, const nchar* fmt,
        const Ts&... args) {
#ifdef BY_DEBUG
        ::by::logger::get().log(lv, filename, func, line, fmt, __convert__((const Ts&) args).unwrap()...);
#endif
    }
} // namespace by
