/// @file
#pragma once

#include "clog/logger.hpp"

namespace by {

    /** @ingroup clog
     *  @brief Rich logging support with polymorphic type conversion
     *  @details Uses polymorphism to convert format specifier arguments into appropriate
     *  types for logging. Since the clog module is positioned at a lower level in the
     *  architecture, it cannot know what classes depend on it. Therefore, richLog is
     *  defined in each module to specify how to convert classes in that module to strings.
     *
     *  richLog consists of two main parts: __convert__ and wrap.
     *
     *  @section convert __convert__
     *  Defines how to convert each module's classes into types that are easy to log.
     *  Callers simply pass their concrete type to __convert__(), and the most appropriate
     *  __convert__ is called via overloading. This means you can define both generic
     *  __convert__() and type-specific __convert__() simultaneously. A notable example is
     *  __convert__(void*), which is called when no other T* matches.
     *
     *  @section wrap wrap
     *  Wrap is needed to unpack values received from __convert__() so they can pass through
     *  variadic arguments. Variadic argument functions can only pass scalar types or T*,
     *  not values. Since some __convert__() functions create new values that need to be
     *  passed by value, wrap is essential as long as logger::log() uses variadic arguments.
     *
     *  __convert__() functions return either @ref strWrap or @ref noWrap<T>. noWrap returns
     *  the received value as-is without processing, while strWrap internally calls
     *  std::string::c_str().
     */
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
