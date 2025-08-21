/// @file
#pragma once

#include "memlite/binder/tbindable.hpp"

struct binderTest;

namespace by {

    class bindTacticable;
    class bindTag;

    /// @ingroup memlite
    /// @brief Smart pointer with loose type checking and lifetime management
    /// @details Provides guaranteed instance cleanup and tracking with three key features:
    /// 1) Distinguished abstract type checking with flexible acceptance/rejection
    /// 2) Complete instance cleanup using RAII
    /// 3) Notification when bound instances are replaced or freed
    /// Prevents process halting and provides developer notifications.
    ///
    /// type checking:
    ///     this was most valuable requirement when I design component of bind.
    ///     in fact, bind was suggested to replace sort of exisiting smart-ptr concept things,
    ///     such as weakptr. because weakptr was planned to replace and enhance existing pointer
    ///     of c/c++, they were very strict to check type and whether template param T was const
    ///     or not. this characteristic was pretty worked good in most environment but not on
    ///     usage of byeol framework.
    ///
    ///     why was it not proper to use as before?:
    ///         byeol is based on class node and make user not need to know what really was.
    ///         all actions (including calling some func on byeol env) can be replacable to
    ///         sending msg using "use()". to say the least, what byeol doing is loose-check.
    ///         so, strict type-checking of existing smart pointers doesn't matched to what
    ///         byeol framework willing to do.
    ///
    /// design:
    ///     binder components are mostly constructed to 3 classes and they form 2 layers
    ///     vertically. class binder:
    ///         represents binder on loose-checking layer.
    ///         user can try any type to bind or get from the binded. compiler won't complain
    ///         about. but because of loose-checking, if it's not proper request, binder will
    ///         warn you at runtime as result.
    ///         constness of binding instance was persisted. (for example, if user try to get
    ///         non-const instance from consted binder, s/he will get nulled reference.)
    ///         user needs to checks that returned value was nullref on using loose-checking
    ///         API.
    ///
    ///     class tweak, class tstr:
    ///         these represent binder on strict-checking layer.
    ///         because it was declared to class template, user need to bind or get binded using
    ///         type T. of course these are based on class 'bind', user can use loose-check API
    ///         case by case.
    class _nout binder: public typeProvidable, public tbindable<instance> {
        BY(ME(binder, instance), INIT_META(me))

        friend class weakTactic;
        friend class strTactic;
        friend class bindTag;       // for _get()
        friend struct ::binderTest; // for UT

    public:
        binder(const type& type, bindTacticable& tactic);
        binder(const me& rhs);
        virtual ~binder();

    public:
        /// @brief Dereference operator to access bound instance
        /// @details This follows the same policy as tmay and stl.
        /// that is, if the binder does not bind any instances and tries to dereference them with
        /// `get()` or `operator*()`, it will behave as UB.
        /// this is likely to crash.
        instance* operator->();
        const instance* operator->() const BY_CONST_FUNC(operator->())
        instance& operator*();
        const instance& operator*() const BY_CONST_FUNC(operator*())
        me& operator=(const me& rhs);

    public:
        //  binder:
        id getItsId() const;
        //  tbindable:
        nbool isBind() const override;
        void rel() override;
        using tbindable::canBind;
        nbool canBind(const type& it) const override;
        using tbindable::bind;
        nbool bind(const instance& it) override;

        instance* get();
        const instance* get() const BY_CONST_FUNC(get())

        template <typename E> E* get() { return get()->template cast<E>(); }
        template <typename E> const E* get() const BY_CONST_FUNC(get<E>())

        //  typeProvidable:
        const type& getType() const override;

        using typeProvidable::cast;
        void* cast(const type& to) override;

    protected:
        nbool _assign(const binder& rhs);
        nbool _onSame(const typeProvidable& rhs) const override;
        bindTag* _getBindTag() const;

    protected:
        id _itsId; // id for binded one
        const type* _type;
        bindTacticable* _tactic;
    };

    // extension for OR macro:
    template <typename F> tmedium<instance> operator|(binder& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return t.get();
    }

    template <typename F> tmedium<instance> operator|(const binder& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return t.get();
    }

    // extension for typeTrait:
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

    // extension for TO macro:
    template <typename T, typename F> auto operator->*(binder* t, F&& f) {
        return t ? f(**t) : typeTrait<std::decay_t<decltype(f(**t))>>::ret();
    }

    template <typename T, typename F> auto operator->*(const binder* t, F&& f) {
        return t ? f(**t) : typeTrait<std::decay_t<decltype(f(**t))>>::ret();
    }

    template <typename T, typename F> auto operator->*(binder& t, F&& f) {
        return t ? f(*t) : typeTrait<std::decay_t<decltype(f(*t))>>::ret();
    }
} // namespace by
