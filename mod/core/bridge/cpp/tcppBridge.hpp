#pragma once

#include "tcppBridgeFunc.hpp"
#include "marshaling/tgenericMarshaling.hpp"
#include "../../ast/obj.hpp"
#include "../../type/mgdType.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "../../ast/defaultCtor.hpp"

namespace nm {

    /// bridge object only can shares 'shared' sub nodes.
    /// @param T represents native class.
    /// @param S represents whether it's generic obj or not.
    template <typename T, typename S = obj, nbool isBaseObj = tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>
    class tcppBridge : public S {
        // TODO: how to impement 'as()' on bridge obj:
        //  each tcppBridge obj has its unique type object. when it got called 'getType()'
        //  it returns its type object.
        //
        //  however, type object is dynamically belongs to this bridge object, when user
        //  tries to get ttype<T>, it's not derived from ntype so it won't have any 'as()'
        //  func. user can't operate conversion in this way.
        NM(CLASS(tcppBridge, S))
        template <typename Ret, typename T1, typename S1, nbool, template <typename, typename, nbool> class Marshaling, typename...Args>
        friend class tcppBridgeFunc;

    public:
        tcppBridge(): super(new mgdType(ttype<T>::get().getName())), _real(nullptr), _ownReal(false) {
            _subs.bind(new scope());
        }
        tcppBridge(T* real): super(new mgdType(ttype<T>::get().getName())), _real(real), _ownReal(true) {
            _subs.bind(new scope());
        }
        tcppBridge(const me& rhs): super(rhs) {
            _real = nul(rhs._real) ? nullptr : new T(*rhs._real);
            _ownReal = false;
            _subs = rhs._subs;
        }
        ~tcppBridge() override {
            if(_ownReal && _real)
                delete _real;
        }

    public:
        // TODO: remove parameter of this func, making 'real' should be handled as a ctor, not here.
        static me& def() { return def(new T()); }
        static me& def(T* real) {
            me* ret = new me(real);
            // TODO: need to handle ctor with argument properly.
            ret->subs().add(baseObj::CTOR_NAME, new defaultCtor(*ret));
            ret->subs().add(baseObj::CTOR_NAME, new defaultCopyCtor(*ret));
            return *ret;
        }

        using super::subs;
        scope& subs() override { return *_subs; }

        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::*fptr)(Args...)) { return funcNonConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::* fptr)(Args...) const) { return funcConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& funcNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tifSub<S, baseObj>::is, tmarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& funcConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tifSub<S, baseObj>::is, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

        T& get() {
            return *_real;
        }
        const T& get() const {
            return *_real;
        }

        const obj& getOrigin() const override {
            // if an object doesn't have owned sub nodes it means that all instances of that classes
            // are same and origin simulteneously.
            return *this;
        }

        // TODO: add defaultCtor

    private:
        T* _real;
        nbool _ownReal;
        tstr<scope> _subs;
    };

    template <typename T, typename S>
    class tcppBridge<T, S, true> {
        // TODO: how to impement 'as()' on bridge obj:
        //  each tcppBridge obj has its unique type object. when it got called 'getType()'
        //  it returns its type object.
        //
        //  however, type object is dynamically belongs to this bridge object, when user
        //  tries to get ttype<T>, it's not derived from ntype so it won't have any 'as()'
        //  func. user can't operate conversion in this way.
        NM(ME(tcppBridge))
        template <typename Ret, typename T1, typename S1, nbool, template <typename, typename, nbool> class Marshaling, typename...Args>
        friend class tcppBridgeFunc;

    private:
        tcppBridge() {}

    public:
        static me def(const T& org) {
            me ret;
            // TODO: add defaultCtor
            ret._subs.add(baseObj::CTOR_NAME, new defaultCtor(org));
            ret._subs.add(baseObj::CTOR_NAME, new defaultCopyCtor(org));
            return ret;
        }

        scope& subs() { return _subs; }
        const scope& subs() const NM_CONST_FUNC(subs())

        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::*fptr)(Args...)) { return funcNonConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::* fptr)(Args...) const) { return funcConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& funcNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            _subs.add(name, new tcppBridgeFunc<Ret, T, S, true, tmarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& funcConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            typedef typename T::super s;
            _subs.add(name, new tcppBridgeFunc<Ret, T, S, true, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

    private:
        scope _subs;
    };
}
