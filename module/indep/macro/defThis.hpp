#pragma once

#include "indep/macro/overload.hpp"

#define __BY__DECL_DEF_ME_2(ME, SUPER) \
    __BY__DECL_DEF_ME_1(BY_UNWRAP(ME)) \
    typedef BY_UNWRAP(SUPER) super;

#define __BY__DECL_DEF_ME_1(ME) typedef BY_UNWRAP(ME) me;

#define __BY__DECL_DEF_ME(...) BY_OVERLOAD(BY_DEF_ME, __VA_ARGS__)

#define BY_DEF_ME_2(ME, SUPER) __BY__DECL_DEF_ME_2(ME, SUPER)
#define BY_DEF_ME_1(ME) __BY__DECL_DEF_ME_1(ME)
#define BY_DEF_ME(...) BY_OVERLOAD(BY_DEF_ME, __VA_ARGS__)
