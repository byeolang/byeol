#pragma once

#include "indep/macro/defThis.hpp"

#define NM_ME_2(ME, SUPER) \
public:                    \
    NM_DEF_ME_2(ME, SUPER) \
private:

#define NM_ME_1(ME) \
public:             \
    NM_DEF_ME_1(ME) \
private:

#define NM_ME(...) NM_OVERLOAD(NM_ME, __VA_ARGS__)

// namuMeta macro's sub-commands:
//  ME: actually it redirects builtin macro 'NM_ME'.
#define __NM__DECL_ME_1(ME) NM_ME_1(ME)
#define __NM__DECL_ME_2(ME, SUPER) NM_ME_2(ME, SUPER)
#define __NM__DECL_ME(...) NM_OVERLOAD(__NM__DECL_ME, __VA_ARGS__)
