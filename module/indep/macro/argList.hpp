/// @file
/// Argument list processing macros for variadic operations
#pragma once

#include "indep/macro/overload.hpp"

//  TODO: improve this using some BY_EACH.
#define BY_LIST_PAIR_X_2(x1, y1) x1
#define BY_LIST_PAIR_X_4(x1, y1, x2, y2) x1, x2
#define BY_LIST_PAIR_X_6(x1, y1, x2, y2, x3, y3) x1, x2, x3
#define BY_LIST_PAIR_X_8(x1, y1, x2, y2, x3, y3, x4, y4) x1, x2, x3, x4
#define BY_LIST_PAIR_X_10(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5) x1, x2, x3, x4, x5
#define BY_LIST_PAIR_X(...) BY_OVERLOAD(BY_LIST_PAIR_X, __VA_ARGS__)

#define BY_LIST_PAIR_Y_2(x1, y1) y1
#define BY_LIST_PAIR_Y_4(x1, y1, x2, y2) y1, y2
#define BY_LIST_PAIR_Y_6(x1, y1, x2, y2, x3, y3) y1, y2, y3
#define BY_LIST_PAIR_Y_8(x1, y1, x2, y2, x3, y3, x4, y4) y1, y2, y3, y4
#define BY_LIST_PAIR_Y_10(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5) y1, y2, y3, y4, y5
#define BY_LIST_PAIR_Y(...) BY_OVERLOAD(BY_LIST_PAIR_Y, __VA_ARGS__)
