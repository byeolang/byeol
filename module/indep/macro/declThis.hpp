/// @file
/// Class declaration macros for me/super type definitions
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

/// @ingroup indep
/// @brief byeolMeta macro's sub-commands, ME
/// @details it defines 2 typedefs, me and super.
/// `me` is same type to `typeof(this)` class in c++.
/// and `super` is literally superclass of `me`.
/// those typedefs are also available in byeol language.
///
/// This is part of an effort to make the environment of the byeol language
/// and the C++ environment as consistent as possible.
#define __BY__DECL_ME_1(ME) BY_ME_1(ME)
#define __BY__DECL_ME_2(ME, SUPER) BY_ME_2(ME, SUPER)
#define __BY__DECL_ME(...) BY_OVERLOAD(__BY__DECL_ME, __VA_ARGS__)
