#pragma once

#include "tcppBridge.hpp"
#include "../../ast/obj.hpp"

namespace nm {

    template <typename T>
    class tgenericCppBridge : public tcppBridge<T> {
        typedef tcppBridge<T> __super__;
        NM(ME(tgenericCppBridge, __super__))

    public:
        static me def() {
            me ret;
            ret._subs.add(baseObj::CTOR_NAME, new defaultCtor(*ret));
            ret._subs.add(baseObj::CTOR_NAME, new defaultCopyCtor(*ret));
            return ret;
        }

        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::* fptr)(Args...)) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, true, tgenericMarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncNonConst(const std::string& name, Ret(T::* fptr)(Args...)) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFunc(const std::string& name, Ret(T::*fptr)(Args...) const) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, true, tgenericMarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& genericFuncConst(const std::string& name, Ret(T::* fptr)(Args...) const) {
            this->subs().add(name, new tcppBridgeFunc<Ret, T, obj, true, tgenericMarshaling, Args...>((Ret(T::*)(Args...)) fptr));
            return *this;
        }
    };
}
