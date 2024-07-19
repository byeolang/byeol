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
        scope& subs() { return _subs; }
        const scope& subs() const NM_CONST_FUNC(subs())

    private:
        scope _subs;
    };

    template <typename T, nbool isBaseObj = tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>
    class tbridger : public baseBridger {
        NM(ME(tbridger, baseBridger))
        template <typename Ret, typename T1, nbool, template <typename, nbool> class Marshaling, typename...Args>
        friend class tbridgeFunc;
        template <typename T1>
        friend class tbridge;

    private:
        tbridger() = default;

    public:
        static me& func(const std::string& name, const baseFunc& bridgeFunc) {
            _me.subs().add(name, bridgeFunc);
            return _me;
        }
        static me& func(const std::string& name, const baseFunc* bridgeFunc) {
            return func(name, *bridgeFunc);
        }

        template <typename... Args>
        static me& ctor() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret(T::*fptr)(Args...)) { return funcNonConst(name, fptr); }
        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret(T::* fptr)(Args...) const) { return funcConst(name, fptr); }
        template <typename Ret, typename... Args>
        static me& funcNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>(fptr));
        }
        template <typename Ret, typename... Args>
        static me& funcConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }
        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>(fptr));
        }
        template <typename Ret, typename... Args>
        me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }
        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
        }
        template <typename Ret, typename... Args>
        me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

        tbridge<T>* make(T* real) {
            return new tbridge(real);
        }

    private:
        inline static me _me;
    };

    template <typename T>
    class tbridger<T, true> : public baseBridger {
        NM(ME(tbridger))

    private:
        tbridger() = default;

    public:
        static me& func(const std::string& name, const baseFunc& bridgeFunc) {
            _me.subs().add(name, bridgeFunc);
            return _me;
        }
        static me& func(const std::string& name, const baseFunc* bridgeFunc) {
            return func(name, *bridgeFunc);
        }

        template <typename... Args>
        static me& ctor() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret(T::*fptr)(Args...)) { return funcNonConst(name, fptr); }
        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret(T::* fptr)(Args...) const) { return funcConst(name, fptr); }
        template <typename Ret, typename... Args>
        static me& funcNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, true, tmarshaling, Args...>(fptr));
        }
        template <typename Ret, typename... Args>
        static me& funcConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            typedef typename T::super s;
            return func(name, new tbridgeFunc<Ret, T, true, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>(fptr));
        }
        template <typename Ret, typename... Args>
        static me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }
        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            return func(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
        }
        template <typename Ret, typename... Args>
        static me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            return func(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

    private:
        inline static me _me;
    };
}
