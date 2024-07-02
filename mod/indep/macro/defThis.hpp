#pragma once

#include "overload.hpp"

#define __NM__DECL_DEF_ME_2(ME, SUPER) \
    __NM__DECL_DEF_ME_1(NM_UNWRAP(ME)) \
    typedef NM_UNWRAP(SUPER) super;

#define __NM__DECL_DEF_ME_1(ME) typedef NM_UNWRAP(ME) me;

#define __NM__DECL_DEF_ME(...) NM_OVERLOAD(NM_DEF_ME, __VA_ARGS__)

#define NM_DEF_ME_2(ME, SUPER) __NM__DECL_DEF_ME_2(ME, SUPER)
#define NM_DEF_ME_1(ME) __NM__DECL_DEF_ME_1(ME)
#define NM_DEF_ME(...) NM_OVERLOAD(NM_DEF_ME, __VA_ARGS__)
