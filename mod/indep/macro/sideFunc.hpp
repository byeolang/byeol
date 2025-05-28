#pragma once

#include "indep/macro/overload.hpp"

#define NM_SIDE_FUNC_2(name, ret) NM_SIDE_FUNC_3(it, name, ret)
#define NM_SIDE_FUNC_3(paramPtr, name, ret) { \
    return paramPtr ? name(*paramPtr) : ret; \
}
#define NM_SIDE_FUNC NM_OVERLOAD(NM_SIDE_FUNC, __VA_ARGS__)
