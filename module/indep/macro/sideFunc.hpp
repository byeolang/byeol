/// @file
/// Side function macros for safe pointer operations
#pragma once

#include "indep/macro/overload.hpp"

#define BY_SIDE_FUNC_1(name) BY_SIDE_FUNC_2(it, name)
#define BY_SIDE_FUNC_2(paramPtr, name) BY_SIDE_FUNC_3(it, name(*it), decltype(name(*it))())
#define BY_SIDE_FUNC_3(paramPtr, expr, ret) \
    { return paramPtr ? expr : ret; }
#define BY_SIDE_FUNC(...) BY_OVERLOAD(BY_SIDE_FUNC, __VA_ARGS__)
