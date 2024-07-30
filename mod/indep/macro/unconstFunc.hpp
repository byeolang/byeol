#pragma once

#include "overload.hpp"

#define NM_CONST_FUNC_2(type, funcCall) \
    {                                   \
        type* uncThis = (type*) this;   \
        return uncThis->funcCall;       \
    }
#define NM_CONST_FUNC_1(funcCall) NM_CONST_FUNC_2(me, funcCall)
#define NM_CONST_FUNC(...) NM_OVERLOAD(NM_CONST_FUNC, __VA_ARGS__)
