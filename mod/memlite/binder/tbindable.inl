#pragma once

#include "tbindable.hpp"

namespace nm {

#define TEMPL template <typename T>
#define ME tbindable<T>

    TEMPL ME::operator nbool() const { return isBind(); }

    TEMPL nbool ME::canBind(const T& it) const {
        if(nul(it)) return false;
        return canBind(it.getType());
    }

    TEMPL
    nbool ME::bind(const T& it) {
        // type checking before binding only is required to bind class.
        // derived classes from this doesn't need it. because its type is specified.
        // prevent wrong type providing by compiler.
        return canBind(it);
    }

    TEMPL
    nbool ME::bind(const T* it) { return bind(*it); }

#undef ME
#undef TEMPL
}
