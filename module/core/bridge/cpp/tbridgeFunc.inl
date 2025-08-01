#pragma once

#include "core/bridge/cpp/tbridge.hpp"
#include "core/bridge/cpp/tbridgeFunc.hpp"

namespace by {

#define TEMPL \
    template <typename Ret, typename T, nbool isBaseObj, template <typename, nbool> class Marshaling, typename... Args>
#define ME tbridgeFunc<Ret, T, isBaseObj, Marshaling, Args...>

    TEMPL
    template <size_t... index> str ME::_marshal(args& a, std::index_sequence<index...> s) {
        tbridge<T>& me = (tbridge<T>*) a.getMe() OR.err("object from frame does not exists.").ret(str());
        WHEN_NUL(me._real).err("this object doesn't have _real.").ret(str());

        return Marshaling<Ret, tifSub<typename typeTrait<Ret>::Org, node>::is>::toMgd(
            (me._real->*(this->_fptr))                                                                   // funcPtr
            (Marshaling<Args, tifSub<typename typeTrait<Args>::Org, node>::is>::toNative(a[index])...)); // and args.
    }

#undef ME
#undef TEMPL
#define TEMPL template <typename T, template <typename, nbool> class Marshaling, typename... Args>
#define ME tbridgeFunc<void, T, false, Marshaling, Args...>

    TEMPL
    template <size_t... index> str ME::_marshal(args& a, std::index_sequence<index...>) {
        tbridge<T>& me = (tbridge<T>*) a.getMe() OR.err("object from frame does not exists.").ret(str());
        WHEN_NUL(me._real).err("this object doesn't have _real.").ret(str());

        (me._real->*(this->_fptr))(
            Marshaling<Args, tifSub<typename typeTrait<Args>::Org, node>::is>::toNative(a[index])...);
        return Marshaling<void, tifSub<void, node>::is>::toMgd();
    }

#undef TEMPL
#undef ME
} // namespace by
