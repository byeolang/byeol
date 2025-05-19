#pragma once

#include "memlite/binder.hpp"

namespace nm {

    template <typename T, typename F>
    const T* operator|(const tstr<T>& t, F&& f) {
        if(!t) return nullptr;
        f(*t);
        return t.get();
    }

    template <typename T, typename F>
    const T* operator|(const tweak<T>& t, F&& f) {
        if(!t) return nullptr;
        f(*t);
        return t.get();
    }
}
