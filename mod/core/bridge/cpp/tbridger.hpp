#pragma once

#include "tcppBridgeFunc.hpp"
#include "marshaling/tgenericMarshaling.hpp"
#include "../../ast/obj.hpp"
#include "../../type/mgdType.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "../../ast/defaultCtor.hpp"

namespace nm {

    class _nout baseBridger {
        NM(ME(baseBridger))

    public:
        scope& subs() { return _subs; }
        const scope& subs() const NM_CONST_FUNC(subs())

    private:
        scope _subs;
    };

    template <typename T, typename S = baseObj, nbool isBaseObj = tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>
    class tbridger : public baseBridger{
        NM(ME(tbridger, baseBridger))
        template <typename Ret, typename T1, typename S1, nbool, template <typename, typename, nbool> class Marshaling, typename...Args>
        friend class tcppBridgeFunc;

    public:
        tbridger(T* org) {
            static tcppBridge<T, S> inner(org);
            // TODO: need to handle ctor with argument properly.
            subs().add(baseObj::CTOR_NAME, new defaultCtor(inner));
            subs().add(baseObj::CTOR_NAME, new defaultCopyCtor(inner));
        }

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

        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tifSub<S, baseObj>::is, tgenericMarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tifSub<S, baseObj>::is, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tifSub<S, baseObj>::is, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tifSub<S, baseObj>::is, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

        tcppBridge<T, S>* make(T* real) {
            return new tcppBridge(subs(), real);
        }
    };

    template <typename T, typename S>
    class tbridger<T, S, true> : public baseBridger {
        NM(ME(tbridger))

    public:
        tbridger(const T& org) {
            subs().add(baseObj::CTOR_NAME, new defaultCtor(org));
            subs().add(baseObj::CTOR_NAME, new defaultCopyCtor(org));
        }

        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::*fptr)(Args...)) { return funcNonConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::* fptr)(Args...) const) { return funcConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& funcNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, true, tmarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& funcConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            typedef typename T::super s;
            subs().add(name, new tcppBridgeFunc<Ret, T, S, true, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, true, tgenericMarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, true, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
    };
}
