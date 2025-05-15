#pragma once

#include "memlite/binder/tstr.hpp"

namespace nm {

    template <typename T> struct nulr<tstr<T>> {
        static nbool isNul(const tstr<T>& it) { return nul(&it) || !it.isBind(); }
    };

    template <typename T> struct nulr<tstr<T>&> {
        static nbool isNul(const tstr<T>& it) { return nul(&it) || !it.isBind(); }
    };
} // namespace nm
