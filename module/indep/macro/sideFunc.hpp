#pragma once

#include "indep/macro/overload.hpp"

#define NM_SIDE_FUNC_2(name, ret) NM_SIDE_FUNC_3(it, name(*it), ret)
#define NM_SIDE_FUNC_3(paramPtr, expr, ret) \
    { return paramPtr ? expr : ret; }
#define NM_SIDE_FUNC(...) NM_OVERLOAD(NM_SIDE_FUNC, __VA_ARGS__)
