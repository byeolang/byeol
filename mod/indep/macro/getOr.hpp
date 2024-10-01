#pragma once

#include "safeGet.hpp"

namespace nm {
    #define orRet
    #define getOr(expr) _PUT(expr); if (__isNul__(_GET(expr))) return
}
