#pragma once

#include "../../ast/baseFunc.hpp"
#include "ast/params.hpp"
#include "bridge/cpp/marshaling/tmarshaling.hpp"
#include "bridge/cpp/tbridgeFunc.hpp"

namespace nm {
    template <typename T, typename... Args>
    class tbridgeCtor : public tbridgeFunc<T, T, tifSub<typename tadaptiveSuper<T>::super, baseObj>::is, tmarshaling, Args...> {
        typedef tbridgeFunc<T, T, tifSub<typename tadaptiveSuper<T>::super, baseObj>::is, tmarshaling, Args...> __super9;
        NM(CLASS(tbridgeCtor, __super9))

    public:
        tbridgeCtor(): super(nullptr) {}

    public:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& a, std::index_sequence<index...>) {
            return tmarshaling<T*, tifSub<T, node>::is>::toMgd(new T(tmarshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...));
        }
    };
}
