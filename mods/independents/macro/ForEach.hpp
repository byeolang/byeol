/*
 * Copyright (C) 2012 William Swanson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or
 * their institutions shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization from the authors.
 */

//    modification: kniz

#pragma once

#include "Helper.hpp"
#include "Evaluator.hpp"

#define _WRD_EACH_GET_END2() 0, WRD_CONSUME_ARGS
#define _WRD_EACH_GET_END1(...) _WRD_EACH_GET_END2
#define _WRD_EACH_GET_END(...) _WRD_EACH_GET_END1
#define _WRD_EACH_NEXT0(test, next, ...) next WRD_VOID()
#define _WRD_EACH_NEXT1(test, next) _WRD_EACH_NEXT0(test, next, 0)
#define _WRD_EACH_NEXT(test, next)  _WRD_EACH_NEXT1(_WRD_EACH_GET_END test, next)

#define _WRD_EACH_LIST_NEXT1(test, next) _WRD_EACH_NEXT0(test, WRD_COMMA next, 0)
#define _WRD_EACH_LIST_NEXT(test, next)  _WRD_EACH_LIST_NEXT1(_WRD_EACH_GET_END test, next)

//    Applies the function macro `f` to each of the remaining parameters.
#define _WRD_EACH0(f, x, peek, ...) f(x) _WRD_EACH_NEXT(peek, _WRD_EACH1)(f, peek, __VA_ARGS__)
#define _WRD_EACH1(f, x, peek, ...) f(x) _WRD_EACH_NEXT(peek, _WRD_EACH0)(f, peek, __VA_ARGS__)
#define WRD_EACH(f, ...) WRD_EVAL(_WRD_EACH1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

//    EACH macro for various parametered function:
//        usage:
//            #define X(x, y) cout << (x+y);
//            WRD_EACH_TUPLE(X, (1,2), (2,3)) // please be careful to wrap with a paranthesis each set of parameters.
//
//        output:
//            37
#define _WRD_EACH_TUPLE0(f, x, peek, ...) f x _WRD_EACH_NEXT(peek, _WRD_EACH_TUPLE1)(f, peek, __VA_ARGS__)
#define _WRD_EACH_TUPLE1(f, x, peek, ...) f x _WRD_EACH_NEXT(peek, _WRD_EACH_TUPLE0)(f, peek, __VA_ARGS__)
#define WRD_EACH_TUPLE(f, ...) WRD_EVAL(_WRD_EACH_TUPLE1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

//    EACH macro for expanding:
//        usage:
//            #define X(x, y) cout << (x+y);
//            int b = 5;
//            WRD_EACH_EXPAND(X, b, 1,2,3)
//            //== cout << (b+1); cout << (b+2); cout << (b+3);
//
//        output:
//            678
#define _WRD_EACH_EXPAND0(f, s, x, peek, ...) f(s, x) _WRD_EACH_NEXT(peek, _WRD_EACH_EXPAND1)(f, s, peek, __VA_ARGS__)
#define _WRD_EACH_EXPAND1(f, s, x, peek, ...) f(s, x) _WRD_EACH_NEXT(peek, _WRD_EACH_EXPAND0)(f, s, peek, __VA_ARGS__)
#define WRD_EACH_EXPAND(f, ...) WRD_EVAL(_WRD_EACH_EXPAND1(f, s, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

//    Applies the function macro `f` to each of the remaining parameters and inserts commas between the results.
#define _WRD_EACH_LIST0(f, x, peek, ...) f(x) _WRD_EACH_LIST_NEXT(peek, _WRD_EACH_LIST1)(f, peek, __VA_ARGS__)
#define _WRD_EACH_LIST1(f, x, peek, ...) f(x) _WRD_EACH_LIST_NEXT(peek, _WRD_EACH_LIST0)(f, peek, __VA_ARGS__)
#define WRD_EACH_LIST(f, ...) WRD_EVAL(_WRD_EACH_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))
