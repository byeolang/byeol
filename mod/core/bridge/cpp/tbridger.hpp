#pragma once

#include "tbridgeFunc.hpp"
#include "marshaling/tgenericMarshaling.hpp"
#include "../../ast/obj.hpp"
#include "../../type/mgdType.hpp"
#include "tbridgeCtor.hpp"

namespace nm {

    class _nout baseBridger {
        NM(ME(baseBridger))

    public:
        virtual ~baseBridger() = default;

    public:
        scope& subs() { return *_subs; }
        const scope& subs() const NM_CONST_FUNC(subs())

    private:
        tstr<scope> _subs = new scope();
    };

    template <typename T, nbool isBaseObj = tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>
    class tbridger : public baseBridger {
        NM(ME(tbridger, baseBridger))
        template <typename Ret, typename T1, nbool, template <typename, nbool> class Marshaling, typename...Args>
        friend class tbridgeFunc;

    public:
        template <typename... Args>
        me& ctor() {
            subs().add(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
            return *this;
        }

        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::*fptr)(Args...)) { return funcNonConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::* fptr)(Args...) const) { return funcConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& funcNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& funcConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            subs().add(name, new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            subs().add(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            subs().add(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

        tbridge<T>* make(T* real) {
            return new tbridge(subs(), real);
        }
    };

    template <typename T>
    class tbridger<T, true> : public baseBridger {
        NM(ME(tbridger))

    public:
        template <typename... Args>
        me& ctor() {
            subs().add(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
            return *this;
        }

        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::*fptr)(Args...)) { return funcNonConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::* fptr)(Args...) const) { return funcConst(name, fptr); }
        template <typename Ret, typename... Args>
        me& funcNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tbridgeFunc<Ret, T, true, tmarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& funcConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            typedef typename T::super s;
            subs().add(name, new tbridgeFunc<Ret, T, true, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            subs().add(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            subs().add(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            subs().add(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
    };
}
