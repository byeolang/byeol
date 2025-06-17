#pragma once

#include "memlite/binder/binder.hpp"
#include "memlite/binder/weakTactic.hpp"

namespace nm {

    class origin;

    template <typename T, typename TACTIC = weakTactic> class tweak: public binder {
        NM_ME(tweak, binder)
        NM_INIT_META(me)
        friend class bindTag;

    public:
        /// tweak:
        tweak();
        tweak(const type& subtype);
        tweak(const T& it);
        tweak(const T* it);
        tweak(const me& rhs);
        explicit tweak(const binder& rhs);

    public:
        T* operator->();
        T& operator*();
        const T* operator->() const NM_CONST_FUNC(operator->())
        const T& operator*() const NM_CONST_FUNC(operator*())
        me& operator=(const binder& rhs);
        me& operator=(const me& rhs);

    public:
        using super::get;
        T* get();
        const T* get() const NM_CONST_FUNC(get())

        using tbindable::bind;
        nbool bind(const T& new1);
    };

    // extension for OR macro:
    template <typename T, typename F> T& operator|(tweak<T>& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t.get();
    }

    template <typename T, typename F> const T& operator|(const tweak<T>& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t.get();
    }

    // extension for typeTrait:
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

    // extension for TO macro:
    template <typename T, typename F> auto operator->*(tweak<T> t, F&& f) {
        return t ? f(*t) : typeTrait<std::decay_t<decltype(f(*t))>>::ret();
    }

    template <typename T, typename F> auto operator->*(tweak<T>& t, F&& f) {
        return t ? f(*t) : typeTrait<std::decay_t<decltype(f(*t))>>::ret();
    }

} // namespace nm
