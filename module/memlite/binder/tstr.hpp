#pragma once

#include "memlite/binder/strTactic.hpp"
#include "memlite/binder/tweak.hpp"

namespace nm {

    template <typename T, typename TACTIC = strTactic> class tstr: public tweak<T, TACTIC> {
        typedef tweak<T, TACTIC> _super_;
        NM_ME(tstr, _super_)
        NM_INIT_META(me)
        friend class ref;

    public:
        //  tstr:
        tstr();
        /// @param subType  subType is should be sub type of 'T' or type 'T' will be used from base
        ///                 class.
        tstr(const type& subtype);
        tstr(const T& it);
        tstr(const T* it);
        tstr(const __medium__<T>& it);
        tstr(const binder& rhs);
        tstr(const me& rhs);

        me& operator=(const me& rhs) = default;
    };

    // extension for OR macro:
    template <typename T, typename F> tmay<T> operator|(tstr<T>& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t.get();
    }
    template <typename T, typename F> tmay<T> operator|(const tstr<T>& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t.get();
    }
    template <typename T, typename F> tmay<T> operator|(tstr<T>&& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t.get();
    }

    // extension for typeTrait:
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

    // extension for TO macro:
    template <typename T, typename F> auto operator->*(tstr<T>& t, F&& f) -> decltype(typeTrait<decltype(f(*t))>::ret()) {
        return t ? f(*t) : typeTrait<decltype(f(*t))>::ret();
    }
    template <typename T, typename F> auto operator->*(const tstr<T>& t, F&& f) -> decltype(typeTrait<decltype(f(*t))>::ret()) {
        return t ? f(*t) : typeTrait<decltype(f(*t))>::ret();
    }
    template <typename T, typename F> auto operator->*(tstr<T>&& t, F&& f) -> decltype(typeTrait<decltype(f(*t))>::ret()) {
        return t ? f(*t) : typeTrait<decltype(f(*t))>::ret();
    }
}
