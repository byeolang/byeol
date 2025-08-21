/// @file
/// Static assertion utilities for compile-time validation
#pragma once

#include <type_traits>
#include "indep/common/typedef.hpp"

namespace by {

    template <nbool...> struct boolPack;
    template <nbool... bp> using allTrues = std::is_same<boolPack<bp..., true>, boolPack<true, bp...>>;
}
