#pragma once

#include "../common/typedef.hpp"
#include <type_traits>

namespace nm {

    template <nbool...> struct boolPack;
    template <nbool... bp>
    using allTrues = std::is_same<boolPack<bp..., true>, boolPack<true, bp...>>;
}
