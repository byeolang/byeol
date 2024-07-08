#pragma once

#include "tstr.hpp"

namespace nm {
    template <typename T>
    static T* _proceed(tstr<T>&& rhs) {
        static tstr<T> a;
        a = rhs;
        return &a.get();
    }
    template <typename T> static T* _proceed(tstr<T>& rhs) { return &rhs.get(); }
    template <typename T> static T* _proceed(tstr<T>* rhs) { return &rhs->get(); }
}
