/// @file
#pragma once

#include "indep/macro/overload.hpp"

#define BY_CONST_FUNC_2(type, funcCall) \
    {                                   \
        type* uncThis = (type*) this;   \
        return uncThis->funcCall;       \
    }
#define BY_CONST_FUNC_1(funcCall) BY_CONST_FUNC_2(me, funcCall)
#define BY_CONST_FUNC(...) BY_OVERLOAD(BY_CONST_FUNC, __VA_ARGS__)
