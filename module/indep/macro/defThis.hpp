/// @file
/// Type definition macros for class self-reference
#pragma once

#include "indep/macro/overload.hpp"

#define __BY__DECL_DEF_ME_2(ME, SUPER) \
    __BY__DECL_DEF_ME_1(BY_UNWRAP(ME)) \
    typedef BY_UNWRAP(SUPER) super;

#define __BY__DECL_DEF_ME_1(ME) typedef BY_UNWRAP(ME) me;

#define __BY__DECL_DEF_ME(...) BY_OVERLOAD(BY_DEF_ME, __VA_ARGS__)

/// @ingroup indep
/// @brief byeolMeta macro's sub-commands, DEF_ME
/// @details Similar to ME, this macro adds typedefs called `me` and `super` to the scope
/// of the current compilation unit.
///
/// This macro was added for use when writing implementation files.
/// Because the `ME` sub-command adds typedefs to the class scope, it's not available
/// for you to access them outside of member fuctions when writing implementation file.
///
/// let me give you example:
/// @code
///     // Foo.cpp --
///
///     BY(DEF_ME(Foo))
///     // From now on, new typedef `me` refers To Foo type, and `super` refers
///     // to Foo::super.
///     // if there's no Foo::super, you'll have some compile errors when use it.
///
///     me::Foo() {
///         // if you didn't use DEF_ME above, `me::Foo() {` wouldn't be possible.
///         // Instead, you should use `Foo::Foo() {` 'cause by the time use `me::`,
///         // it's not in a member funtion.
///     }
/// @endcode
#define BY_DEF_ME_2(ME, SUPER) __BY__DECL_DEF_ME_2(ME, SUPER)
#define BY_DEF_ME_1(ME) __BY__DECL_DEF_ME_1(ME)
#define BY_DEF_ME(...) BY_OVERLOAD(BY_DEF_ME, __VA_ARGS__)
