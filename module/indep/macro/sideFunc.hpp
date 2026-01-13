/// @file
#pragma once

#include "indep/macro/overload.hpp"

#define BY_SIDE_FUNC_1(name) BY_SIDE_FUNC_2(it, name)
#define BY_SIDE_FUNC_2(paramPtr, name) BY_SIDE_FUNC_3(it, name(*it), decltype(name(*it))())
#define BY_SIDE_FUNC_3(paramPtr, expr, ret) \
    { return paramPtr ? expr : ret; }

/** @ingroup indep
 *  @brief Side function macros for safe pointer operations
 *  @details Side Func is a term I coined.
 *  It refers to an overloaded function added for the convenience of the original function.
 *  For example:
 *  @code
 *      void foo(std::string name, std::string value);
 *
 *      // This is a side func.
 *      void foo(std::string value) { return foo("default", value); }
 *  @endcode
 *  This macro makes it easy to create side functions like the one above.
 */
#define BY_SIDE_FUNC(...) BY_OVERLOAD(BY_SIDE_FUNC, __VA_ARGS__)
