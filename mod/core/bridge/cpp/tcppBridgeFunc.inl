#pragma once

#include "tcppBridgeFunc.hpp"
#include "tcppBridge.hpp"
#include "../../frame/thread.hpp"
#include "../../ast/params.hpp"

namespace nm {

#define TEMPL template <typename Ret, typename T, typename S, template <typename, typename, nbool> class Marshaling, typename... Args>
#define ME tcppBridgeFunc<Ret, T, S, Marshaling, Args...>

    TEMPL
    template <size_t... index>
    str ME::_marshal(args& a, std::index_sequence<index...> s) {
        auto* me = (tcppBridge<T, S>*) &a.getMe();
        if(nul(me)) return NAMU_E("object from frame does not exists."), str();
        if(me->template isSub<mockNode>()) {
            mockNode& mock = me->template cast<mockNode>();
            me = (tcppBridge<T, S>*) &mock.getTarget();
        }

        return Marshaling<Ret, S, tifSub<Ret, node>::is>::toMgd((me->_real->*(this->_fptr)) // funcptr
                (Marshaling<Args, S, tifSub<Args, node>::is>::toNative(a[index])...)); // and args.
    }

#undef ME
#undef TEMPL
#define TEMPL template <typename T, typename S, template <typename, typename, nbool> class Marshaling, typename... Args>
#define ME tcppBridgeFunc<void, T, S, Marshaling, Args...>

    TEMPL
    template <size_t... index>
    str ME::_marshal(args& a, std::index_sequence<index...>) {
        auto* me = (tcppBridge<T, S>*) &a.getMe();
        if(nul(me)) return NAMU_E("object from frame does not exists."), str();
        if(me->template isSub<mockNode>()) {
            mockNode& mock = me->template cast<mockNode>();
            me = (tcppBridge<T, S>*) &mock.getTarget();
        }

        (me->_real->*(this->_fptr))(Marshaling<Args, S, tifSub<Args, node>::is>::toNative(a[index])...);
        return Marshaling<void, S, tifSub<void, node>::is>::toMgd();
    }

#undef TEMPL
#undef ME
}
