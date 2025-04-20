/// @file   NM_OVERLOAD.hpp
/// @brief  Make it possible to provide overloaded macro.
/// @remark These are codes inspired from Boost library. added the PATCH for VS.
#pragma once

#include "indep/macro/argCounter.hpp"
#include "indep/macro/helper.hpp"

/// Implementing Macro Overloding:
///     by count of given arguments, let compiler determine which macro should works.
///     code inspired by Boost library, patched for visual studio's weird varadic macro.
//      branch to macro that would be expanded by its argument count:
//
//      usage:
//          //  define macro:
//          #define MyMacro_0() 100
//          #define MyMacro_1(x) (x)+5
//          #define MyMacro_2(x, y) (x)+(y)
//          #define MyMacro(...)    NM_OVERLOAD(MyMacro, __VA_ARGS__)
//
//          //  using:
//          cout << MyMacro(5) << ", " << MyMacro(3, 5) << ", " << MyMacro() << "\n";
//
//      output:
//          10, 8, 100
#define _OVERLOAD(macro_name, argcount) NM_CONCAT(macro_name, argcount)
#define NM_OVERLOAD(macro_name, ...) \
    _OVERLOAD(macro_name, NM_CONCAT(_, NM_ARGCNT(__VA_ARGS__)))(__VA_ARGS__)
