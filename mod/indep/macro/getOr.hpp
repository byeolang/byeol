#pragma once

#include "safeGet.hpp"

namespace nm {
    #define orRet
    #define orNul(T) nulOf<T>()
    #define getOr(expr) _PUT(expr); if (__isNul__(_GET(expr))) return
}
