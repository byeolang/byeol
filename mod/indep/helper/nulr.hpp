#pragma once

#include "indep/helper/tmay.hpp"
#include <type_traits>

namespace nm {


    template <typename T> struct nulr {
        static_assert(!std::is_reference_v<T>, "error: reference type specialization not allowed");
        static bool isNul(T) { return false; }
    };

    template <typename T> struct nulr<T*> {
        static nbool isNul(const T* it) { return it == nullptr; }
    };

    template <typename T> struct nulr<tmay<T>> {
        static nbool isNul(const tmay<T>& it) { return nul(&it) || !it.has(); }
    };

    template <typename T> struct nulr<tmay<T>&> {
        static nbool isNul(const tmay<T>& it) { return !it.has(); }
    };

    template <typename T>
    auto nul(T&& it) -> decltype(nulr<std::decay_t<T>>::isNul(std::forward<T>(it))) {
        return nulr<std::decay_t<T>>::isNul(std::forward<T>(std::forward<T>(it)));
    }
} // namespace nm
