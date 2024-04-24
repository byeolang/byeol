#pragma once

#include "tcppBridgeFunc.hpp"
#include "tcppBridge.hpp"
#include "../../frame/thread.hpp"
#include "../../ast/params.hpp"

namespace wrd {

#define TEMPL template <typename Ret, typename T, typename... Args>
#define ME tcppBridgeFuncBase<Ret, T, Args...>

    TEMPL const params& ME::getParams() const {
        static params* inner = nullptr;
        if(nul(inner)) {
            inner = new params();
            (inner->add(new param("", ttype<typename tmarshaling<Args>::mgdType>::get())), ...);
        }

        return *inner;
    }

#undef ME
#define ME tcppBridgeFunc<Ret, T, Args...>

    TEMPL
    template <size_t... index>
    str ME::_marshal(narr& args, std::index_sequence<index...>) {
        auto& me = (tcppBridge<T>&) frame::getObj();
        if(nul(me)) return WRD_E("object from frame does not exists."), str();

        return tmarshaling<Ret>::toMgd((me._real->*(this->_fptr))(tmarshaling<Args>::toNative(args[index])...));
    }

#undef ME
#undef TEMPL
#define TEMPL template <typename T, typename... Args>
#define ME tcppBridgeFunc<void, T, Args...>

    TEMPL
    template <size_t... index>
    str ME::_marshal(narr& args, std::index_sequence<index...>) {
        const frame& fr = thread::get().getNowFrame();
        auto& me = (tcppBridge<T>&) frame::getObj();
        if(nul(me)) return WRD_E("object from frame does not exists."), str();

        (me._real->*(this->_fptr))(tmarshaling<Args>::toNative(args[index])...);
        return tmarshaling<void>::toMgd();
    }

#undef TEMPL
#undef ME
}
