#pragma once

#include "indep/macro/defThis.hpp"

#define BY_ME_2(ME, SUPER) \
public:                    \
    BY_DEF_ME_2(ME, SUPER) \
private:

#define BY_ME_1(ME) \
public:             \
    BY_DEF_ME_1(ME) \
private:

#define BY_ME(...) BY_OVERLOAD(BY_ME, __VA_ARGS__)

// byeolMeta macro's sub-commands:
//  ME: actually it redirects builtin macro 'BY_ME'.
#define __BY__DECL_ME_1(ME) BY_ME_1(ME)
#define __BY__DECL_ME_2(ME, SUPER) BY_ME_2(ME, SUPER)
#define __BY__DECL_ME(...) BY_OVERLOAD(__BY__DECL_ME, __VA_ARGS__)
