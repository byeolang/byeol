/// @file
#pragma once

#include "core/ast/baseFunc.hpp"
#include "core/ast/params.hpp"
#include "core/bridge/cpp/marshaling/tmarshaling.hpp"
#include "core/bridge/cpp/tbridgeFunc.hpp"

namespace by {
    /** @ingroup core
     *  @brief Bridge constructor for C++ objects
     *  @details Template constructor bridge that creates C++ objects from byeol constructor calls.
     */
    template <typename T, template <typename, nbool> class Marshaling = tmarshaling, typename... Args>
    class tbridgeCtor
        : public tbridgeFunc<T, T, tifSub<typename tadaptiveSuper<T>::super, baseObj>::is, Marshaling, Args...> {
        typedef tbridgeFunc<T, T, tifSub<typename tadaptiveSuper<T>::super, baseObj>::is, Marshaling, Args...>
            __super9;
        BY(ME(tbridgeCtor, __super9), CLONE(tbridgeCtor))

    public:
        tbridgeCtor(): super(nullptr) {}

    public:
        str _runNative(args& args) override { return _marshal(args, std::index_sequence_for<Args...>()); }

        template <size_t... index> str _marshal(args& a, std::index_sequence<index...>) {
            return Marshaling<T*, tifSub<T, node>::is>::toMgd(
                new T(Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...));
        }
    };
}
