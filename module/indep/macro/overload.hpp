/** @file   BY_OVERLOAD.hpp
 *  @brief  Make it possible to provide overloaded macro.
 *  @remark These are codes inspired from Boost library. added the PATCH for VS.
 */
#pragma once

#include "indep/macro/helper.hpp"

/** @ingroup indep
 *  @brief Macro Overloding:
 *  @details by count of given arguments, let compiler determine which macro should works.
 *   original source code from BuvinJ at
 *   https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
 *
 *   usage:
 *   @code
 *       //  define macro:
 *       #define MyMacro_0() 100
 *       #define MyMacro_1(x) (x)+5
 *       #define MyMacro_2(x, y) (x)+(y)
 *       #define MyMacro(...) BY_OVERLOAD(MyMacro, __VA_ARGS__)
 *
 *       //  using:
 *       cout << MyMacro(5) << ", " << MyMacro(3, 5) << ", " << MyMacro() << "\n";
 *   @endcode
 *
 *   expected output should be, 10, 8, 100.
 */
#define BY_OVERLOAD(NAME, ...) BY_CONCAT(NAME##_, __OVERLOAD_VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define __OVERLOAD_HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define __OVERLOAD_HAS_NO_COMMA(...) _ARG16(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#define _TRIGGER_PARENTHESIS_(...) ,

#define __OVERLOAD_HAS_ZERO_OR_ONE_ARG(...)                                                                         \
    ___OVERLOAD_HAS_ZERO_OR_ONE_ARG(       /* test if there is just one argument, eventually an empty one           \
                                            */                                                                      \
        __OVERLOAD_HAS_COMMA(__VA_ARGS__), /* test if _TRIGGER_PARENTHESIS_ together with the                       \
                                              argument adds a comma */                                              \
        __OVERLOAD_HAS_COMMA(                                                                                       \
            _TRIGGER_PARENTHESIS_ __VA_ARGS__), /* test if the argument together with a parenthesis adds a comma */ \
        __OVERLOAD_HAS_COMMA(__VA_ARGS__(~)),   /* test if placing it between _TRIGGER_PARENTHESIS_                 \
                                                   and the parenthesis adds a comma */                              \
        __OVERLOAD_HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__(~)))

#define __OVERLOAD_PASTE5(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4
#define ___OVERLOAD_HAS_ZERO_OR_ONE_ARG(_0, _1, _2, _3) \
    __OVERLOAD_HAS_NO_COMMA(__OVERLOAD_PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,

#define _VA0(...) __OVERLOAD_HAS_ZERO_OR_ONE_ARG(__VA_ARGS__)
#define _VA1(...) __OVERLOAD_HAS_ZERO_OR_ONE_ARG(__VA_ARGS__)
#define _VA2(...) 2
#define _VA3(...) 3
#define _VA4(...) 4
#define _VA5(...) 5
#define _VA6(...) 6
#define _VA7(...) 7
#define _VA8(...) 8
#define _VA9(...) 9
#define _VA10(...) 10
#define _VA11(...) 11
#define _VA12(...) 12
#define _VA13(...) 13
#define _VA14(...) 14
#define _VA15(...) 15
#define _VA16(...) 16

#define __OVERLOAD_VA_NUM_ARGS(...) __OVERLOAD_VA_NUM_ARGS_IMPL(__VA_ARGS__, __OVERLOAD_RSEQ_N(__VA_ARGS__))
#define __OVERLOAD_VA_NUM_ARGS_IMPL(...) __OVERLOAD_VA_NUM_ARGS_N(__VA_ARGS__)

#define __OVERLOAD_VA_NUM_ARGS_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

#define __OVERLOAD_RSEQ_N(...)                                                                           \
    _VA16(__VA_ARGS__), _VA15(__VA_ARGS__), _VA14(__VA_ARGS__), _VA13(__VA_ARGS__), _VA12(__VA_ARGS__),  \
        _VA11(__VA_ARGS__), _VA10(__VA_ARGS__), _VA9(__VA_ARGS__), _VA8(__VA_ARGS__), _VA7(__VA_ARGS__), \
        _VA6(__VA_ARGS__), _VA5(__VA_ARGS__), _VA4(__VA_ARGS__), _VA3(__VA_ARGS__), _VA2(__VA_ARGS__),   \
        _VA1(__VA_ARGS__), _VA0(__VA_ARGS__)
