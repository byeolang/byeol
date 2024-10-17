#pragma once

#include "ttype.hpp"
#include "type.hpp"

namespace nm {

#define ME type

    template <typename T> nint ME::isSuper() const { return isSuper(ttype<T>::get()); }

    template <typename T> nint ME::isSub() const { return ttype<T>::get().isSuper(*this); }

#undef ME
}
