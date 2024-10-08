#pragma once

#include "tbridge.hpp"
#include "tbridgeFunc.hpp"

namespace nm {

#define TEMPL                                            \
    template <typename Ret, typename T, nbool isBaseObj, \
        template <typename, nbool> class Marshaling, typename... Args>
#define ME tbridgeFunc<Ret, T, isBaseObj, Marshaling, Args...>

    TEMPL
    template <size_t... index> str ME::_marshal(args& a, std::index_sequence<index...> s) {
        auto *me = (tbridge<T>*) &a.getMe() orRet NM_E("object from frame does not exists."),
             str();
        if(nul(me->_real)) return NM_E("this object doesn't have _real."), str();

        return Marshaling<Ret, tifSub<Ret, node>::is>::toMgd((me->_real->*(this->_fptr)) // funcptr
            (Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...)); // and args.
    }

#undef ME
#undef TEMPL
#define TEMPL template <typename T, template <typename, nbool> class Marshaling, typename... Args>
#define ME tbridgeFunc<void, T, false, Marshaling, Args...>

    TEMPL
    template <size_t... index> str ME::_marshal(args& a, std::index_sequence<index...>) {
        auto *me = (tbridge<T>*) &a.getMe() orRet NM_E("object from frame does not exists."),
             str();
        if(nul(me->_real)) return NM_E("this object doesn't have _real."), str();

        (me->_real->*(this->_fptr))(
            Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...);
        return Marshaling<void, tifSub<void, node>::is>::toMgd();
    }

#undef TEMPL
#undef ME
} // namespace nm
