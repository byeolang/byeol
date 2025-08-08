#pragma once

//    Funnel:
//        This macro mostly used to put in arguments to macro behind macro.
//
//        Why does we need this?:
//            #define My_2(X, y) .......
//
//            But, if you put class template as macro argument, expanding macro gets messed up.
//                My_2(template<int, float>, template<A, B>) // in fact, preprocessor thought we 
//                    passed 4 arguments, not 2.
//
//        Then, Use Funnel macro instead:
//            My_2(BY_PAIR(MyMap<T, U>), BY_PAIR(template <typename T, typename U))
#include "indep/macro/helper.hpp"
#include "indep/macro/overload.hpp"

#define BY_PAIR_1(x) x
#define BY_PAIR_2(x, y) x, y
#define BY_PAIR_3(x, y, z) BY_PAIR_2(x, y), z
#define BY_PAIR_4(x, y, z, a) BY_PAIR_2(x, y), BY_PAIR_2(z, a)
#define BY_PAIR(...) BY_OVERLOAD(BY_PAIR, __VA_ARGS__)
