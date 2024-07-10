#pragma once

#include "logger.hpp"

namespace nm {

    template <typename T>
    struct noWrap {
        noWrap(T rhs): data(rhs) {}

        T unwrap() const {
            return data;
        }

        T data;
    };

    template <typename T>
    struct useWrap {
        static_assert(true, "not supported wrapper for richLog");
    };

    struct _nout strWrap {
        strWrap(const std::string& rhs);
        const nchar* unwrap() const;
        std::string data;
    };

    _nout noWrap<nint> __convert__(nint rhs);
    _nout noWrap<nuint> __convert__(nuint rhs);
    _nout noWrap<nflt> __convert__(nflt rhs);
    _nout noWrap<nchar> __convert__(nchar rhs);
    _nout strWrap __convert__(nbool rhs);
    _nout noWrap<const nchar*> __convert__(const nchar* rhs);
    _nout noWrap<ndbl> __convert__(ndbl rhs);
    _nout noWrap<nlong> __convert__(nlong rhs);
    _nout noWrap<nulong> __convert__(nulong rhs);
    _nout noWrap<nint64> __convert__(nint64 rhs);
    _nout noWrap<nuint64> __convert__(nuint64 rhs);
    _nout strWrap __convert__(const std::string& rhs);
    _nout strWrap __convert__(void* rhs);

    template <typename... Ts>
    void richLog(logLv::level lv, const std::string& filename, const nchar* func, nint line, const nchar* fmt, const Ts&... args) {
        ::nm::logger::get().log(lv, filename, func, line, fmt, __convert__((const Ts&) args).unwrap()...);
    }
}
