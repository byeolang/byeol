#pragma once

#include "indep/helper/tmay.hpp"

namespace nm {

    template <typename T> struct nulr {
        static_assert(
            !std::is_reference_v<T>, 
            "error: reference type specialization not allowed"
        );

        static T get() { return T(); }

        static nbool isNul(T) { return false; }
    };

    template <typename T> struct nulr<T*> {
        static T* get() { return nullptr; }

        static nbool isNul(const T* it) { return it == nullptr; }
    };

    template <typename T> struct nulr<tmay<T>> {
        static tmay<T> get() { return tmay<T>(); }

        static nbool isNul(const tmay<T>& it) { return nul(&it) || !it.has(); }
    };

    template <typename T> struct nulr<tmay<T>&> {
        static tmay<T>& get() {
            static tmay<T> inner;
            return inner;
        }

        static nbool isNul(const tmay<T>& it) { return !it.has(); }
    };

    template <typename T>
    inline typename std::enable_if<!std::is_pointer_v<T>, nbool>::type nul(const T& it) {
        return nulr<T>::isNul(it);
    }

    template <typename T> inline nbool nul(const T* it) { return nulr<T*>::isNul(it); }
} // namespace nm
