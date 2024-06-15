#pragma once

#include "builtin/container/mgd/arr.hpp"
#include "tmarshaling.hpp"

namespace namu {

#define TEMPL template <typename T, typename S>
#define ME tmarshaling<tarr<T, T>, S, true>

    TEMPL
    tarr<T, T> ME::toNative(node& it) {
        return tarr<T, T>(it.cast<arr>());
    }

    TEMPL
    template <typename E>
    str ME::toMgd(E* it) {
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
