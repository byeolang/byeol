#pragma once

#include "overload.hpp"
#include "forEach.hpp"

// namu universal DECL macro:
//  NM_DECL is generalized API used to describe the metadata of class in namu.
//  NM_DECL can be used to define detailed metadata about a class by chaining sub-command sets.
//  with NM macro, I can clarify that those INIT_META, VISIT are should be after of NM macro.
//  and limit the scope of availbility.
//  these macros which are available only inside of NM macro are called to sub-commands.
//
// Usage:
//  use NM_DECL macro at declaration of your class.
//
//  class Foo {
//      NM_DECL(cmd1(arg1, arg2, ...), cmd2(arg1, arg2, ...), ...)
//
//  public:
//      ...and your codes...
//  };

#define _ON_EACH_DECL(cmd) __NM__DECL_##cmd
#define NM(...) NM_EACH(_ON_EACH_DECL, __VA_ARGS__)
