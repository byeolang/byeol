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
#include "core/ast/ctor.hpp"

namespace by {

    /** @ingroup core
     *  @brief C++ to byeol bridge builder
     *  @details As the name suggests, tbridger handles the bridge role between C++ native environment and byeol
     *  managed environment. Specifically, tbridger handles the `cpp <--> byeol` bridge. The goal is to easily expose
     *  natively defined functions or classes as managed types or functions with just a few lines of C++ code.
     *
     *  @remark tbridge is designed as a component unit rather than a single class. If you just use it, there's no big
     *  problem, but if you try to understand the code without experience in this area, it might be confusing. So let's
     *  look at basic usage first to get a feel, then examine the detailed structure.
     *
     *  @remark It's recommended to review @ref baseObj or @ref obj first.
     *
     *  @section basic_usage Basic Usage
     *  See baseObj for a complete usage example with nStr. The key is that tbridger can very easily expose
     *  native-written functions to managed code.
     *
     *  @section design_structure Design Structure
     *  Brief introduction to core classes in the bridge component. See each class for detailed operation methods and
     *  algorithms.
     *
     *  - **tbridger** is the entry point for the bridge component, performing a Facade role. tbridger is a class
     *    template that takes each C++ class as a type parameter. Through functions like `func()`, it stores
     *    externally defined functions in static variable `subs()`.
     *
     *  - **tbridge** is a baseObj created using subs registered in tbridger as origin. Internally it defines origin as
     *    `tbaseObjOrigin<tbridger<T>>`, so after filling subs() through tbridger and calling tbridge::getOrigin(), the
     *    tbridge object possesses those functions through origin.
     *
     *  - **tbridgeFunc** creates a managed environment baseFunc instance that redirects general C++ member function
     *    pointers. Calling tbridger::func() internally creates a tbridgeFunc instance like `tbridgerFunc<....>()`.
     *    tbridgeFunc internally applies marshaling to convert C++ native types (node*, int, etc.) to appropriate
     *    managed types (str, nInt). The same marshaling applies when exporting return values.
     *
     *  - **@ref tmarshaling** handles the marshaling mentioned above.
     *
     *  - **tbridgeCtor** represents functions for constructors. @ref ctor in managed environment is just a constructor
     *    function and doesn't participate in object creation. This separates the roles of instance creation and
     *    constructor invocation, making logic for calling parent class constructors in inheritance relationships
     *    concise. However, tbridgeCtor must represent constructors according to C++ syntax, making it difficult to
     *    separately split object creation through new and constructor invocation. Therefore, tbridgeCtor handles both
     *    constructor function redirection and object creation through the `new` keyword.
     *
     *  - **tbridgeClosure** helps expose C++ lambda functions through tbridger. The principle itself isn't much
     *    different from tbridgeFunc. It just handles lambdas as std::function instead of function pointers.
     */
    template <typename T, nbool isBaseObj = tifSub<typename tadaptiveSuper<T>::super, baseObj>::is> class tbridger {
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

        static me& func(const std::string* name, const baseFunc& bridgeFunc)
            BY_SIDE_FUNC(name, func(*name, bridgeFunc), _get());
        static me& func(const std::string& name, const baseFunc* bridgeFunc)
            BY_SIDE_FUNC(bridgeFunc, func(name, *bridgeFunc), _get());
        static me& func(const std::string* name, const baseFunc* bridgeFunc)
            BY_SIDE_FUNC(name&& bridgeFunc, func(*name, *bridgeFunc), _get());

        template <typename... Args> static me& ctor() {
            return func(func::CTOR_NAME, new tbridgeCtor<T, tmarshaling, Args...>());
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

        static me& genericCtor() { return func(func::CTOR_NAME, new tbridgeCtor<T, tgenericMarshaling>()); }

        template <typename... Args> static me& genericCtor() {
            return func(func::CTOR_NAME, new tbridgeCtor<T, tgenericMarshaling, Args...>());
        }

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

        static tbridge<T>* make(T* real) { return coreInternal::makeBridge(real); }

    private:
        static me& _get() {
            static me _inner;
            return _inner;
        }

    private:
        scope _subs;
    };

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

        static me& ctor() { return func(func::CTOR_NAME, new tbridgeCtor<T>()); }

        template <typename... Args> static me& ctor() {
            return func(func::CTOR_NAME, new tbridgeCtor<T, tmarshaling, Args...>());
        }

        /*template <typename T1, typename... Args> static me& ctorIndirect() {
            return func(func::CTOR_NAME, new tbridgeCtor<T1, Args...>());
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
