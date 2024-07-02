#pragma once

#include "overload.hpp"

#define NM_UNCONST_FUNC_2(type, funcCall) { \
        type* uncThis = const_cast<type*>(this); \
        return uncThis->funcCall; \
    }
#define NM_UNCONST_FUNC_1(funcCall) NM_UNCONST_FUNC_2(me, funcCall);
#define NM_UNCONST_FUNC(...) NM_OVERLOAD(NM_UNCONST_FUNC, __VA_ARGS__)
