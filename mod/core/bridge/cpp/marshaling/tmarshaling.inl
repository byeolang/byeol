#pragma once

#include "builtin/container/mgd/arr.hpp"
#include "tmarshaling.hpp"

namespace nm {

#define TEMPL template <typename T, typename E, typename S>
#define ME tmarshaling<tarr<T, E>, S, true>

    TEMPL
    tarr<T, E> ME::toNative(node& it) {
        return tarr<T, E>(it.cast<arr>());
    }

    TEMPL
    template <typename E2>
    str ME::toMgd(E2* it) {
        return new arr(*it);
    }

    TEMPL
    typename ME::mgd& ME::onAddParam() {
        return *new mgd(tmarshaling<T, S>::onAddParam());
    }

    TEMPL
    typename ME::mgd& ME::onGetRet() {
        return *new mgd();
    }

#undef ME
#undef TEMPL

}
