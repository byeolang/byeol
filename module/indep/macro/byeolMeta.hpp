/// @file
/// Core metadata declaration macro system for byeol language classes
#pragma once

#include "indep/macro/forEach.hpp"
#include "indep/macro/overload.hpp"

// byeol universal DECL macro:
//  BY_DECL is generalized API used to describe the metadata of class in byeol.
//  BY_DECL can be used to define detailed metadata about a class by chaining sub-command sets.
//  with BY macro, I can clarify that those INIT_META, VISIT are should be after of BY macro.
//  and limit the scope of availbility.
//  these macros which are available only inside of BY macro are called to sub-commands.
//
// Usage:
//  use BY_DECL macro at declaration of your class.
//
//  class Foo {
//      BY_DECL(cmd1(arg1, arg2, ...), cmd2(arg1, arg2, ...), ...)
//
//  public:
//      ...and your codes...
//  };

#define _ON_EACH_DECL(cmd) __BY__DECL_##cmd
#define BY(...) BY_EACH(_ON_EACH_DECL, __VA_ARGS__)
