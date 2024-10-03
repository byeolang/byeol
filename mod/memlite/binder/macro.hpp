#pragma once

#include "tstr.hpp"

namespace nm {
    template <typename T> static T* __proceed__(const tweak<T>&& rhs) {
        static tweak<T> a;
        a = rhs;
        return &a.get();
    }

    template <typename T> static T* __proceed__(tweak<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(tweak<T>* rhs) { return &rhs->get(); }

    template <typename T> static T* __proceed__(const tweak<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(const tweak<T>* rhs) { return &rhs->get(); }

    template <typename T> static T* __proceed__(const tstr<T>&& rhs) {
        static tstr<T> a;
        a = rhs;
        return &a.get();
    }

    template <typename T> static T* __proceed__(tstr<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(tstr<T>* rhs) { return &rhs->get(); }

    template <typename T> static T* __proceed__(const tstr<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(const tstr<T>* rhs) { return &rhs->get(); }

    template <typename T, typename TACTIC> static nbool __isNul__(const tstr<T, TACTIC>&& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tstr<T, TACTIC>& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tstr<T, TACTIC>* rhs) {
        return !rhs || !*rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tweak<T, TACTIC>&& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tweak<T, TACTIC>& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tweak<T, TACTIC>* rhs) {
        return !rhs || !*rhs;
    }

    template <typename T> struct __to_ref__<tstr<T>> {
        static T& to(tstr<T> it) { return *it; }
    };

    template <typename T> struct __to_ref__<tstr<T>&> {
        static T& to(tstr<T>& it) { return *it; }
    };

    template <typename T> struct __to_ref__<tstr<T>*> {
        static T& to(tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __to_ref__<tstr<T>&&> {
        static T& to(tstr<T>&& it) { return *it; }
    };
} // namespace nm
