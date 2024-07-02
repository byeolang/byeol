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
//            My_2(NM_PAIR(MyMap<T, U>), NM_PAIR(template <typename T, typename U))
#include "helper.hpp"
#include "overload.hpp"

#define NM_PAIR_1(x)           x
#define NM_PAIR_2(x, y)        x, y
#define NM_PAIR_3(x, y, z)     NM_PAIR_2(x, y), z
#define NM_PAIR_4(x, y, z, a)  NM_PAIR_2(x, y) , NM_PAIR_2(z, a)
#define NM_PAIR(...)           NM_OVERLOAD(NM_PAIR, __VA_ARGS__)
