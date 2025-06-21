#pragma once

#include "indep/macro/overload.hpp"

#define NM_SIDE_FUNC_1(name) NM_SIDE_FUNC_2(it, name)
#define NM_SIDE_FUNC_2(paramPtr, name) NM_SIDE_FUNC_3(it, name(*it), decltype(name(*it))())
#define NM_SIDE_FUNC_3(paramPtr, expr, ret) { return paramPtr ? expr : ret; }
#define NM_SIDE_FUNC(...) NM_OVERLOAD(NM_SIDE_FUNC, __VA_ARGS__)
