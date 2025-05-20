#pragma once

#include "memlite/binder.hpp"

namespace nm {

    template <typename T, typename F>
    const T& operator|(const tstr<T>& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t.get();
    }

    template <typename T, typename F>
    const T& operator|(const tweak<T>& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t.get();
    }
}
