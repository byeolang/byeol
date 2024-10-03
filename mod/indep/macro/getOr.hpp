#pragma once

#include "then.hpp"

namespace nm {

    template <typename T> class _tget {
    public:
        static T& set(const T& expr) { return *store() = (T*) &expr, get(); }

        static T& get() { return **store(); }

        static T** store() {
            static T* inner = 0; // NOLINT
            return &inner;
        }
    };

    template <typename T> class _tget<T*> {
    public:
        static T* set(const T* trg) { return *store() = (T*) trg; }

        static T* get() { return *store(); }

        static T** store() {
            static T* inner = 0; // NOLINT
            return &inner;
        }
    };

    template <typename T> class _tget<T&> {
    public:
        static T& set(const T& expr) { return *store() = (T*) &expr, get(); }

        static T& get() { return **store(); }

        static T** store() {
            static T* inner = 0; // NOLINT
            return &inner;
        }
    };

    template <typename T> static nbool __isNul__(const T&& rhs) { return nul(rhs); }

    template <typename T> static nbool __isNul__(const T& rhs) { return nul(rhs); }

    template <typename T> static nbool __isNul__(const T* rhs) { return nul(rhs); }

#define _PUSH(exp) _tget<decltype(exp)>::set(exp)
#define _GET(exp) _tget<decltype(exp)>::get()
#define _NULR(exp) nulOf<decltype(exp)>()

#define orRet return
#define orNul(T) return nulOf<T>()
#define orDo
#define getOr(expr) \
    _PUSH(expr);    \
    if(__isNul__(_GET(expr)))

} // namespace nm
