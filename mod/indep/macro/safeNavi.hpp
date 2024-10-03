#pragma once

#include "../common/typedef.hpp"
#include "../helper/nulr.hpp"
#include "overload.hpp"
#include <vector>

namespace nm {

    template <typename T>
    struct __empty__ {
        static T ret() { return T{}; }
    };
    template <typename T>
    struct __empty__<T&> {
        static T& ret() {
            T* ret = nullptr;
            return *ret;
        }
    };
    template <>
    struct __empty__<void> {
        static void ret() {}
    };

    template <typename T, typename F>
    auto operator->*(T&& t, F&& f) {
      return f(std::forward<T>(t));
    }

#define __pcall__(fn)                                                            \
      [&](auto&& __p) {                                                          \
        if constexpr (std::is_pointer_v<std::remove_reference_t<decltype(__p)>>) \
          return __p ? __p->fn : __empty__<decltype(__p->fn)>::ret(); \
        else                                                                     \
          return &__p ? __p.fn : __empty__<decltype(__p.fn)>::ret(); \
      }
#define THEN ->*__pcall__



    template <typename T> class _tget {
    public:
        static T& push(const T& expr) {
            inner.push_back((T*) &expr);
            return (T&) expr;
        }

        static T& get() { return inner.empty() ? nulOf<T>() : *inner.back(); }

        static T& pop() {
            T& ret = get();
            inner.pop_back();
            return ret;
        }

    private:
        static inline std::vector<T*> inner; // NOLINT
    };

    template <typename T> class _tget<T*> {
    public:
        static T* push(const T* trg) {
            inner.push_back((T*) trg);
            return (T*) trg;
        }

        static T* get() { return inner.empty() ? nullptr : inner.back(); }

        static T& pop() {
            T* ret = get();
            inner.pop_back();
            return *ret;
        }

    private:
        static inline std::vector<T*> inner; // NOLINT
    };

    template <typename T> class _tget<T&> {
    public:
        static T& push(const T& expr) {
            inner.push_back((T*) &expr);
            return (T&) expr;
        }

        static T& get() { return inner.empty() ? nulOf<T>() : *inner.back(); }

        static T& pop() {
            T& ret = get();
            inner.pop_back();
            return ret;
        }

    private:
        static inline std::vector<T*> inner; // NOLINT
    };

    template <typename T> static T* ___proceed__(T&& rhs) {
        static T a;
        a = rhs;
        return &a;
    }

    template <typename T> static T* __proceed__(T& rhs) { return &rhs; }

    template <typename T> static T* __proceed__(T* rhs) { return rhs; }

    template <typename T> static nbool __isNul__(const T&& rhs) { return nul(rhs); }

    template <typename T> static nbool __isNul__(const T& rhs) { return nul(rhs); }

    template <typename T> static nbool __isNul__(const T* rhs) { return nul(rhs); }
} // namespace nm

#define _PUSH(exp) _tget<decltype(exp)>::push(exp)
#define _GET(exp) _tget<decltype(exp)>::get()
#define _POP(exp) _tget<decltype(exp)>::pop()
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
#define safeGet_1(e1) (__isNul__(_PUSH(e1)) ? _NULR(e1) : (_POP(e1)))
#define safeGet_2(e1, e2) \
    (__isNul__(_PUSH(e1)) ? _NULR(__proceed__(e1)->e2) : __proceed__(_POP(e1))->e2)

#define safeGet_3(e1, e2, e3) \
    (__isNul__(_PUSH(safeGet_2(e1, e2))) ? _NULR(__proceed__(__proceed__(e1)->e2)->e3) : \
                                    __proceed__(_POP(__proceed__(e1)->e2))->e3)
#define safeGet_4(e1, e2, e3, e4)                                          \
    (__isNul__(_PUSH(safeGet_3(e1, e2, e3))) ?                             \
            _NULR(__proceed__(__proceed__(__proceed__(e1)->e2)->e3)->e4) : \
            __proceed__(_POP(__proceed__(__proceed__(e1)->e2)->e3))->e4)

#define safeGet_5(e1, e2, e3, e4, e5) \
    (__isNul__(_PUSH(safeGet_4(e1, e2, e3, e4))) ? \
        _NULR(__proceed__(__proceed__(__proceed__(__proceed__(e1)->e2)->e3)->e4)->e5) : \
        __proceed__(_POP(__proceed__(__proceed__(__proceed__(e1)->e2)->e3)->e4))->e5)

#define safeGet(...) NM_OVERLOAD(safeGet, __VA_ARGS__)
