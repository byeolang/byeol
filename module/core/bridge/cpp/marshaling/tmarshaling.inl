#pragma once

#include "core/builtin/container/mgd/arr.hpp"
#include "core/bridge/cpp/marshaling/tmarshaling.hpp"

namespace nm {

#define TEMPL template <typename T, typename E>
#define ME tmarshaling<tarr<T, E>, true>

    TEMPL
    tarr<T, E> ME::toNative(node& it) {
        arr& cast = it.cast<arr>() OR.ret(tarr<T, E>());
        return tarr<T, E>(cast);
    }

    TEMPL
    template <typename E2> str ME::toMgd(E2* it) { return new arr(*it); }

    TEMPL
    const typename ME::mgd& ME::onAddParam() { return *new mgd(*new T()); }

    TEMPL
    const typename ME::mgd* ME::onGetRet() { return new mgd(*new T()); }

#undef ME
#undef TEMPL

}
