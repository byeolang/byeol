#pragma once

#include <type_traits>
#include "indep/common/typedef.hpp"

namespace nm {

    template <typename T> struct typeTrait {
        typedef T Org;
        typedef T& Ref;
        typedef T* Ptr;

        static T ret() { return T{}; } // return default value

        static bool isNul(T) { return false; }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <typename T> struct typeTrait<T&> {
        typedef T Org;
        typedef T& Ref;
        typedef T* Ptr;

        static nbool isNul(const T& it) { return true; }

        static T& ret() {
            static T dummy;
            return dummy;
        }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = true;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <> struct typeTrait<void> {
        static void ret() {}

        static bool isNul(void) { return false; }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <typename T> struct typeTrait<T*> {
        typedef T Org;
        typedef T& Ref;
        typedef T* Ptr;

        static nbool isNul(const T* it) { return it == nullptr; }

        static T* ret() { return nullptr; }

        static constexpr nbool is_ptr = true;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <> struct typeTrait<void*> {
        typedef void Org;
        typedef void* Ptr;

        static nbool isNul(const void* it) { return it == nullptr; }

        static void* ret() { return nullptr; }

        static constexpr nbool is_ptr = true;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <typename T>
    auto nul(T&& it) -> decltype(typeTrait<std::remove_reference_t<std::decay_t<T>>>::isNul(it)) {
        return typeTrait<std::remove_reference_t<std::decay_t<T>>>::isNul(std::forward<T>(it));
    }
} // namespace nm
