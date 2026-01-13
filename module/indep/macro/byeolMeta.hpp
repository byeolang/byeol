/** @file
 *  Core metadata declaration macro system for byeol language classes
 */
#pragma once

#include "indep/macro/forEach.hpp"
#include "indep/macro/overload.hpp"

/** @ingroup indep
 *  @brief byeol universal macro
 *  @details This reduces the possibility of macro conflicts between different
 *  libraries and makes macro writing easier.
 *  and this is generalized API used to describe the metadata of class in byeol.
 *  BY can be used to define detailed metadata about a class by chaining sub-command sets.
 *  these sub-commands are macros available only inside of BY.
 *
 *  for instance,
 *  @code
 *   class Foo {
 *       BY(CLASS(Foo), VISIT(Foo), ...)
 *
 *   public:
 *       ...and your codes...
 *   };
 *  @endcode
 *
 *  Each command preceding BY is prefixed with the __BY__DECL_ prefix.
 *  The following commands can be used within the BY macro:
 *      CLASS:  Injects metadata for a concrete class.
 *      VISIT:  Makes the class visitor-friendly.
 *      ME:     Adds typedefs named super and me.
 *      DEF_ME: Same as ME, but used in the implementation file.
 *      ADT:    Injects metadata for an abstract class.
 *      CLONE:  Adds a virtual copy constructor.
 */
#define _ON_EACH_DECL(cmd) __BY__DECL_##cmd
#define BY(...) BY_EACH(_ON_EACH_DECL, __VA_ARGS__)
