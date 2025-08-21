/// @file
#pragma once

#include "core/ast/obj.hpp"
#include "core/ast/tbaseObjOrigin.hpp"
#include "core/type/mgdType.hpp"
#include "core/bridge/cpp/marshaling/tgenericMarshaling.hpp"
#include "core/bridge/cpp/tbridgeClosure.hpp"
#include "core/bridge/cpp/tbridgeCtor.hpp"
#include "core/bridge/cpp/tbridgeFunc.hpp"
#include "core/ast/src/dumSrcFile.hpp"

namespace by {

    /// @ingroup core
    /// @brief Bridge builder for C++ classes
    /// @details Template utility for building bridges between C++ classes and byeol object system.
    template <typename T, nbool isBaseObj = tifSub<typename tadaptiveSuper<T>::super, baseObj>::is> class tbridger {
        BY(ME(tbridger))
        template <typename Ret, typename T1, nbool, template <typename, nbool> class Marshaling, typename... Args>
        friend class tbridgeFunc;
        template <typename T1> friend class tbridge;

    private:
        tbridger() = default;

    public:
        static scope& subs() { return _get()._subs; }

        static me& extend(const scope& src) {
            subs().link(src);
            return _get();
        }

        static me& func(const std::string& name, const baseFunc& bridgeFunc) {
            _get().subs().add(name, bridgeFunc);
            return _get();
        }

        static me& func(const std::string* name, const baseFunc& bridgeFunc)
            BY_SIDE_FUNC(name, func(*name, bridgeFunc), _get());
        static me& func(const std::string& name, const baseFunc* bridgeFunc)
            BY_SIDE_FUNC(bridgeFunc, func(name, *bridgeFunc), _get());
        static me& func(const std::string* name, const baseFunc* bridgeFunc)
            BY_SIDE_FUNC(name&& bridgeFunc, func(*name, *bridgeFunc), _get());

        template <typename... Args> static me& ctor() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
        }

        template <typename Ret, typename... Args> static me& func(const std::string& name, Ret (T::*fptr)(Args...)) {
            return funcNonConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string* name, Ret (T::*fptr)(Args...)) BY_SIDE_FUNC(name, func(*name, fptr), _get());

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return funcConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string* name, Ret (T::*fptr)(Args...) const)
            BY_SIDE_FUNC(name, func(*name, fptr), _get());

        template <typename Ret, typename... Args>
        static me& funcNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>(fptr));
        }

        template <typename Ret, typename... Args>
        static me& funcNonConst(const std::string* name, Ret (T::*fptr)(Args...))
            BY_SIDE_FUNC(name, funcNonConst(*name, fptr), _get());

        template <typename Ret, typename... Args>
        static me& funcConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& funcConst(const std::string* name, Ret (T::*fptr)(Args...) const)
            BY_SIDE_FUNC(name, funcConst(*name, fptr), _get());

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>(fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string* name, Ret (T::*fptr)(Args...))
            BY_SIDE_FUNC(name, genericFunc(*name, fptr), _get());

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string* name, Ret (T::*fptr)(Args...) const)
            BY_SIDE_FUNC(name, genericFunc(*name, fptr), _get());

        template <typename Ret, typename... Args>
        static me& genericFuncNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFuncNonConst(const std::string* name, Ret (T::*fptr)(Args...))
            BY_SIDE_FUNC(name, genericFuncNonConst(*name, fptr), _get());

        template <typename Ret, typename... Args>
        static me& genericFuncConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFuncConst(const std::string* name, Ret (T::*fptr)(Args...) const)
            BY_SIDE_FUNC(name, genericFuncConst(*name, fptr), _get());

        template <typename Ret, typename T1 = T, typename... Args>
        static me& closure(const std::string& name, std::function<Ret(T1&, Args...)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling, Args...>(c));
        }

        template <typename Ret, typename T1 = T, typename... Args>
        static me& closure(const std::string* name, std::function<Ret(T1&, Args...)> c)
            BY_SIDE_FUNC(name, closure(*name, c), _get());

        template <typename Ret, typename T1 = T>
        static me& closure(const std::string& name, std::function<Ret(T1&)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling>(c));
        }

        template <typename Ret, typename T1 = T>
        static me& closure(const std::string* name, std::function<Ret(T1&)> c)
            BY_SIDE_FUNC(name, closure(*name, c), _get());

        static tbridge<T>* make(T* real) { return new tbridge(real); }

    private:
        static me& _get() {
            static me _inner;
            return _inner;
        }

    private:
        scope _subs;
    };

    /// @ingroup core
    /// @brief Bridge builder specialization for baseObj-derived classes
    /// @details Specialized bridge builder for C++ classes that inherit from baseObj.
    template <typename T> class tbridger<T, true> {
        BY(ME(tbridger))

    private:
        tbridger() = default;

    public:
        static scope& subs() { return _get()._subs; }

        static me& extend(const scope& src) {
            subs().link(src);
            return _get();
        }

        static me& func(const std::string& name, const baseFunc& bridgeFunc) {
            _get().subs().add(name, bridgeFunc);
            return _get();
        }

        static me& func(const std::string& name, const baseFunc* bridgeFunc) { return func(name, *bridgeFunc); }

        static me& ctor() { return func(baseObj::CTOR_NAME, new tbridgeCtor<T>()); }

        template <typename... Args> static me& ctor() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
        }

        /*template <typename T1, typename... Args> static me& ctorIndirect() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T1, Args...>());
        }*/

        template <typename Ret, typename T1 = T, typename... Args>
        static me& closure(const std::string& name, std::function<Ret(T1&, Args...)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling, Args...>(c));
        }

        template <typename Ret, typename T1 = T>
        static me& closure(const std::string& name, std::function<Ret(T1&)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling>(c));
        }

        template <typename Ret, typename... Args> static me& func(const std::string& name, Ret (T::*fptr)(Args...)) {
            return funcNonConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return funcConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& funcNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, true, tmarshaling, Args...>(fptr));
        }

        template <typename Ret, typename... Args>
        static me& funcConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            typedef typename T::super s;
            auto* fun = new tbridgeFunc<Ret, T, true, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr);
            fun->setSrc(*new src(dumSrcFile::singleton(), name, point{0, 0}));
            return func(name, fun);
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...)) {
            auto* fun = new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>(fptr);
            fun->setSrc(*new src(dumSrcFile::singleton(), name, point{0, 0}));
            return func(name, fun);
        }

        template <typename Ret, typename... Args>
        static me& genericFuncNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            auto* fun = new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr);
            fun->setSrc(*new src(dumSrcFile::singleton(), name, point{0, 0}));
            return func(name, fun);
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...) const) {
            auto* fun = new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr);
            fun->setSrc(*new src(dumSrcFile::singleton(), name, point{0, 0}));
            return func(name, fun);
        }

        template <typename Ret, typename... Args>
        static me& genericFuncConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            auto* fun = new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr);
            fun->setSrc(*new src(dumSrcFile::singleton(), name, point{0, 0}));
            return func(name, fun);
        }

    private:
        static me& _get() {
            static me _inner;
            return _inner;
        }

    private:
        scope _subs;
    };
} // namespace by
