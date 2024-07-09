#pragma once

#include "tstr.hpp"

namespace nm {
    template <typename T>
    static T* __proceed__(tstr<T>&& rhs) {
        static tstr<T> a;
        a = rhs;
        return &a.get();
    }
    template <typename T> static T* __proceed__(tstr<T>& rhs) { return &rhs.get(); }
    template <typename T> static T* __proceed__(tstr<T>* rhs) { return &rhs->get(); }

    template <typename T> static nbool __isNul__(tstr<T>&& rhs) { return nul(rhs) || !rhs; }
    template <typename T> static nbool __isNul__(tstr<T>& rhs) { return nul(rhs) || !rhs; }
    template <typename T> static nbool __isNul__(tstr<T>* rhs) { return nul(rhs) || !*rhs;  }
}
