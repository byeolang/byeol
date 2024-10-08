#pragma once

#include "tstr.hpp"

namespace nm {

    template <typename T> struct nulr<tstr<T>> {
        static tstr<T> get() { return tstr<T>(); }

        static nbool isNul(const tstr<T>& it) { return nul(&it) || !it.isBind(); }
    };

    template <typename T> struct nulr<tstr<T>&> {
        static tstr<T>& get() {
            tstr<T>* ret = nullptr;
            return *ret;
        }

        static nbool isNul(const tstr<T>& it) { return nul(&it) || !it.isBind(); }
    };

    template <typename T> struct __unwrap_binder__<tstr<T>> {
        static T& to(tstr<T> it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tstr<T>&> {
        static T& to(tstr<T>& it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tstr<T>*> {
        static T& to(tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __unwrap_binder__<tweak<T>> {
        static T& to(tstr<T> it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tweak<T>&> {
        static T& to(tstr<T>& it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tweak<T>*> {
        static T& to(tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __to_ref__<tstr<T>> {
        static T& to(tstr<T> it) { return *it; }
    };

    template <typename T> struct __to_ref__<tstr<T>&> {
        static T& to(tstr<T>& it) { return *it; }
    };

    template <typename T> struct __to_ref__<tstr<T>*> {
        static T& to(tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __to_ref__<const tstr<T>&> {
        static const T& to(const tstr<T>& it) { return *it; }
    };

    template <typename T> struct __to_ref__<const tstr<T>*> {
        static const T& to(const tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __to_ref__<tstr<T>&&> {
        static T& to(tstr<T>&& it) { return *it; }
    };
} // namespace nm
