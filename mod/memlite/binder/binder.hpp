#pragma once

#include "tbindable.hpp"

struct binderTest;

namespace nm {

    class bindTacticable;
    class bindTag;

    /// bind:
    ///     overview:
    ///         can guarrantee that specific instance freed completely and track of them.
    ///         this has 3 features.
    ///             1) can distringuish what given abstract type actually was and reject or accept
    ///             it
    ///                regarding that.
    ///             2) free instance completely. using RAII.
    ///             3) can notice that binding instance was replaced or freed without unexpected
    ///             exception.
    ///         in conlusion, it prevent halting process or give a chance to let developer be
    ///         advised.
    ///
    ///     type cheking:
    ///         this was most valuable requirement when I design component of bind.
    ///         in fact, bind was suggested to replace sort of exisiting smart-ptr concept things,
    ///         such as weakptr. because weakptr was planned to replace and enhance existing pointer
    ///         of c/c++, they were very strict to check type and whether template param T was const
    ///         or not. this characteristic was pretty worked good in most environment but not on
    ///         usage of namu framework.
    ///
    ///         why was it not proper to use as before?:
    ///             namu is based on class node and make user not need to know what really was.
    ///             all actions (including calling some func on namu env) can be replacable to
    ///             sending msg using "use()". to say the least, what namu doing is loose-check. so,
    ///             strict type-checking of existing smart pointers doesn't matched to what namu
    ///             framework willing to do.
    ///
    ///     design:
    ///         binder components are mostly constructed to 3 classes and they form 2 layers
    ///         vertically. class binder:
    ///             represents binder on loose-checking layer.
    ///             user can try any type to bind or get from the binded. compiler won't complain
    ///             about. but because of loose-checking, if it's not proper request, binder will
    ///             warn you at runtime as result.
    ///             constness of binding instance was persisted. (for example, if user try to get
    ///             non-const instance from consted binder, s/he will get nulled reference.)
    ///             user needs to checks that returned value was nullref on using loose-checking
    ///             API.
    ///
    ///         class tweak, class tstr:
    ///             these represent binder on strict-checking layer.
    ///             because it was declared to class template, user need to bind or get binded using
    ///             type T. of course these are based on class 'bind', user can use loose-check API
    ///             case by case.
    class _nout binder: public typeProvidable, public tbindable<instance> {
        NM(ME(binder, instance), INIT_META(me))

        friend class weakTactic;
        friend class strTactic;
        friend class bindTag;       // for _get()
        friend struct ::binderTest; // for UT

    public:
        binder(const type& type, bindTacticable& tactic);
        binder(const me& rhs);
        virtual ~binder();

    public:
        instance* operator->();
        instance& operator*();
        const instance* operator->() const NM_CONST_FUNC(operator->())
        const instance& operator*() const NM_CONST_FUNC(operator*())
        me& operator=(const me& rhs);

    public:
        //  binder:
        id getItsId() const;
        //  tbindable:
        nbool isBind() const override;
        void rel() override;
        using tbindable::canBind;
        nbool canBind(const type& cls) const override;
        using tbindable::bind;
        nbool bind(const instance& it) override;

        instance& get();
        const instance& get() const NM_CONST_FUNC(get())

        template <typename E> E& get() {
            instance& got = get();
            if(nul(got)) return nulOf<E>();

            return got.template cast<E>();
        }
        template <typename E> const E& get() const NM_CONST_FUNC(get<E>())

        //  typeProvidable:
        const type& getType() const override;

        using typeProvidable::cast;
        void* cast(const type& to) override;

    protected:
        nbool _assign(const binder& rhs);
        nbool _onSame(const typeProvidable& rhs) const override;
        bindTag& _getBindTag() const;

    protected:
        id _itsId; // id for binded one
        const type* _type;
        bindTacticable* _tactic;
    };
} // namespace nm
