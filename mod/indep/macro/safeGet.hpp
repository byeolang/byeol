#pragma once

#include "overload.hpp"
#include "../common/typedef.hpp"
#include "../helper/nulr.hpp"

namespace nm {
    template <typename T>
    class _tget {
    public:
        static T& set(const T& expr) { return *store() = (T*) &expr, get(); }
        static T& get() { return **store(); }
        static T** store() {
            static T* inner = 0;
            return &inner;
        }
    };
    template <typename T>
    class _tget<T*> {
    public:
       static T* set(const T* trg) { return *store() = (T*) trg; }
       static T* get() { return *store(); }
       static T** store() {
           static T* inner = 0;
           return &inner;
       }
    };
    template <typename T>
    class _tget<T&> {
    public:
        static T& set(const T& expr) { return *store() = (T*) &expr, get(); }
        static T& get() { return **store(); }
        static T** store() {
            static T* inner = 0;
            return &inner;
        }
    };

    template <typename T>
    static T* ___proceed__(T&& rhs) {
        static T a;
        a = rhs;
        return &a;
    }
    template <typename T> static T* __proceed__(T& rhs) { return &rhs; }
    template <typename T> static T* __proceed__(T* rhs) { return rhs; }

    template <typename T> static nbool __isNul__(T&& rhs) { return nul(rhs); }
    template <typename T> static nbool __isNul__(T& rhs) { return nul(rhs); }
    template <typename T> static nbool __isNul__(T* rhs) { return nul(rhs); }
}

#define _PUT(exp) _tget<decltype(exp)>::set(exp)
#define _GET(exp) _tget<decltype(exp)>::get()
#define _NULR(exp) nulOf<decltype(exp)>()

/// safeGet is safe navigation feature of c++:
/// usage:
///     as-is:
///         ```cpp
///             int getBrushColorCode(Resource r) {
///                 auto& pallete = r.getPallete();
///                 if(nul(pallete)) {
///                     log("pallete is null");
///                     return -1;
///                 }
///
///                 auto& canvas = pallete.getCanvas();
///                 if(nul(canvas)) {
///                     log("canvas is null");
///                     return -1;
///                 }
///
///                 auto& brush = canvas.getBrush(BushType.SYSTEM);
///                 if(nul(brush)) {
///                     log("brush is null");
///                     return -1;
///                 }
///
///                 return brush.getColorCode();
///             }
///         ```
///
///     to be:
///         ```cpp
///             int getBrushColorCode(Resource r) {
///                 int& code = safeGet(r, getPallete(), getCanvas(), getBrush(), getColorCode());
///                 if(nul(code))
///                     return log("code is null"), -1;
///                 return code;
///             }
///       ```
#define safeGet_1(e1) (__isNul__(_PUT(e1)) ? _NULR(e1) : (_GET(e1)))
#define safeGet_2(e1, e2) (__isNul__(_PUT(e1)) ? _NULR((__proceed__(e1)->e2)) : _PUT(__proceed__(_GET(e1))->e2))
#define safeGet_3(e1, e2, e3) (__isNul__(safeGet_2(e1, e2)) ? _NULR(__proceed__(__proceed__(e1)->e2)->e3) : _PUT(__proceed__(_GET(__proceed__(e1)->e2))->e3))
#define safeGet_4(e1, e2, e3, e4) (__isNul__(safeGet_3(e1, e2, e3)) ? _NULR(__proceed__(__proceed__(__proceed__(e1)->e2)->e3)->e4) : _PUT(__proceed__(_GET(__proceed__(__proceed__(e1)->e2)->e3))->e4))
#define safeGet_5(e1, e2, e3, e4, e5) (__isNul__(safeGet_4(e1, e2, e3, e4)) ? _NULR(__proceed__(__proceed__(__proceed__(__proceed__(e1)->e2)->e3)->e4)->e5) : _PUT(__proceed__(_GET(__proceed__(__proceed__(__proceed__(e1)->e2)->e3)->e4)->e5))
#define safeGet(...) NM_OVERLOAD(safeGet, __VA_ARGS__)
