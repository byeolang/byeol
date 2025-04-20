#pragma once

#include "meta/type/ttype.hpp"
#include "meta/type/type.hpp"

namespace nm {

#define ME type

    template <typename T> nbool ME::isSuper() const { return isSuper(ttype<T>::get()); }

    template <typename T> nbool ME::isSub() const { return ttype<T>::get().isSuper(*this); }

#undef ME
}
