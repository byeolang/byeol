#pragma once

#include "overload.hpp"

namespace nm {
    template <typename T>
    class _TGet {
    public:
        static T& set(const T& expr) { return *store() = (T*) &expr, get(); }
        static T& get() { return **store(); }
        static T** store() {
            static T* inner = 0;
            return &inner;
        }
    };
    template <typename T>
    class _TGet<T*> {
    public:
       static T* set(const T* trg) { return *store() = (T*) trg; }
       static T* get() { return *store(); }
       static T** store() {
           static T* inner = 0;
           return &inner;
       }
    };
    template <typename T>
    class _TGet<T&> {
    public:
        static T& set(const T& expr) { return *store() = (T*) &expr, get(); }
        static T& get() { return **store(); }
        static T** store() {
            static T* inner = 0;
            return &inner;
        }
    };
}

#define _PUT(exp) _TGet<TypeTrait<decltype(exp)>::Org>::set(exp)
#define _GET(exp) _TGet<TypeTrait<decltype(exp)>::Org>::get()
#define _NULR(exp) nulOf<TypeTrait<decltype(exp)>::Org>()

#define NAMU_GETS_1(e1) (nul(_PUT((e1))) ? _NULR((e1)) : (_GET((e1))))
#define NAMU_GETS_2(e1, e2) (nul(_PUT((e1))) ? _NULR((e1.e2)) : _PUT(_GET(e1).e2))
#define NAMU_GETS_3(e1, e2, e3) (nul(NAMU_GETS_2(e1, e2)) ? _NULR(e1.e2.e3) : _PUT((_GET(e1.e2).e3)))
#define NAMU_GETS_4(e1, e2, e3, e4) (nul(NAMU_GETS_3(e1, e2, e3)) ? _NULR(e1.e2.e3.e4) : _PUT((_GET(e1.e2.e3).e4)))
//#define NAMU_GETS_4(e1, e2, e3, e4) _PUT(e1).isNull() ? _NULR(e1.e2.e3.e4) : (_PUT(e1.e2).isNull() ? _NULR(e1.e2.e3.e4) : (_PUT(e1.e2.e3).isNull() ? _NULR(e1.e2.e3.e4) : _PUT(e1.e2.e3.e4)))
#define NAMU_GETS(...)   NAMU_OVERLOAD(NAMU_GETS, __VA_ARGS__)
