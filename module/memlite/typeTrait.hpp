#pragma once

#include "memlite/binder.hpp"

namespace nm {

    template <typename T> struct typeTrait<tstr<T>> {
        typedef tstr<T> Org;
        typedef tstr<T>& Ref;
        typedef tstr<T>* Ptr;

        static tstr<T> ret() { return tstr<T>{}; } // return default value

        static nbool isNul(const tstr<T>& it) { return !it.isBind(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = true;
    };

    template <typename T> struct typeTrait<tstr<T>*> {
        typedef tstr<T> Org;
        typedef tstr<T>& Ref;
        typedef tstr<T>* Ptr;

        static tstr<T>* ret() { return nullptr; }

        static nbool isNul(const tstr<T>* it) { return !it || !it->isBind(); }

        static constexpr nbool is_ptr = true;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <typename T> struct typeTrait<tstr<T>&> {
        typedef tstr<T> Org;
        typedef tstr<T>& Ref;
        typedef tstr<T>* Ptr;

        static nbool isNul(const tstr<T>& it) { return !it.isBind(); }

        static tstr<T>& ret() {
            static tstr<T> dummy;
            return dummy;
        }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = true;
        static constexpr nbool is_like_ptr = true;
    };

    template <typename T> struct typeTrait<tweak<T>> {
        typedef tweak<T> Org;
        typedef tweak<T>& Ref;
        typedef tweak<T>* Ptr;

        static tweak<T> ret() { return tweak<T>{}; } // return default value

        static nbool isNul(const tweak<T>& it) { return !it.isBind(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = true;
    };

    template <typename T> struct typeTrait<tweak<T>*> {
        typedef tweak<T> Org;
        typedef tweak<T>& Ref;
        typedef tweak<T>* Ptr;

        static tweak<T>* ret() { return nullptr; }

        static nbool isNul(const tweak<T>* it) { return !it || !it->isBind(); }

        static constexpr nbool is_ptr = true;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <typename T> struct typeTrait<tweak<T>&> {
        typedef tweak<T> Org;
        typedef tweak<T>& Ref;
        typedef tweak<T>* Ptr;

        static nbool isNul(const tweak<T>& it) { return !it.isBind(); }

        static tweak<T>& ret() {
            static tweak<T> dummy;
            return dummy;
        }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = true;
        static constexpr nbool is_like_ptr = true;
    };

    template <> struct typeTrait<binder> {
        typedef binder Org;
        typedef binder& Ref;
        typedef binder* Ptr;

        static nbool isNul(const binder& it) { return !it.isBind(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = true;
    };

    template <> struct typeTrait<binder*> {
        typedef binder Org;
        typedef binder& Ref;
        typedef binder* Ptr;

        static binder* ret() { return nullptr; }

        static nbool isNul(const binder* it) { return !it || !it->isBind(); }

        static constexpr nbool is_ptr = true;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <> struct typeTrait<binder&> {
        typedef binder Org;
        typedef binder& Ref;
        typedef binder* Ptr;

        static nbool isNul(const binder& it) { return it.isBind(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = true;
        static constexpr nbool is_like_ptr = true;
    };
} // namespace nm
