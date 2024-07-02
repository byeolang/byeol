#pragma once

#include "tcppBridge.hpp"
#include "../../ast/obj.hpp"

namespace nm {

    template <typename T>
    class tgenericCppBridge : public tcppBridge<T, obj> {
        typedef tcppBridge<T, obj> __super__;
        NAMU(CLASS(tgenericCppBridge, __super__))

    public:
        tgenericCppBridge(): super() {}
        tgenericCppBridge(T* real): super(real) {}

    public:
        static me& def() {
            me* ret = new me(new T());
            // TODO: need to handle ctor with argument.
            ret->subs().add(baseObj::CTOR_NAME, new defaultCtor(*ret));
            ret->subs().add(baseObj::CTOR_NAME, new defaultCopyCtor(*ret));
            return *ret;
        }

        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, tgenericMarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }

    };
}
