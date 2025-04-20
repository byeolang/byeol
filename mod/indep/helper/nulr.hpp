#pragma once

#include "indep/helper/tmay.hpp"

namespace nm {

    template <typename T> struct nulr {
        static T get() { return T(); }

        static nbool isNul(T) { return false; }
    };

    template <typename T> struct nulr<T&> {
        static T& get() {
            T* ret = 0;
            return *ret;
        }

        static nbool isNul(const T& it) {
            // reference-null-check hack:
            //  in c++ standard reference, it's not allow to check pointer of reference whether
            //  it's null or not, it's always regarded to not null.
            //
            //  however, in namu project, I decided to use only reference for some reason and
            //  treat it like a pointer. instead, I won't declare a pointer at all.
            //
            //  so we need to check nulity of a reference but compiler always optimize it to true
            //  when it meets checking of a pointer to a reference.
            //  but if we do some pointer arithmetic to an address of nulled reference, it won't be
            //  optimized.
            //  and we need to add '1' as a pointer to char type. because it's the only type that
            //  we can guarantee it'll be always size 1 when this software is built on any platform
            //  or architecture.
            return ((char*) &it) + 1 == (char*) 1;
        }
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

        static nbool isNul(const tmay<T>& it) { return nul(&it) || !it.has(); }
    };

    /// short version of nulr.
    template <typename T> T& nulOf() { return nulr<T&>::get(); }

    template <typename T>
    inline typename std::enable_if<!std::is_pointer_v<T>, nbool>::type nul(const T& it) {
        return nulr<T&>::isNul(it);
    }

    template <typename T> inline nbool nul(const T* it) { return nulr<T*>::isNul(it); }
} // namespace nm
