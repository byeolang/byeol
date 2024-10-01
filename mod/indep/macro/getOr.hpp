#pragma once

#include "safeGet.hpp"

namespace nm {
    #define orRet return
    #define orNul(T) return nulOf<T>()
    #define orDo
    #define getOr(expr) _PUT(expr); if (__isNul__(_GET(expr)))
}
