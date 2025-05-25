#pragma once

#include "indep/common/typedef.hpp"
#include "indep/macro/overload.hpp"
#include <vector>
#include <type_traits>

namespace nm {


    template <typename T> struct __empty__ {
        static T ret() { return T{}; } // 값 타입은 기본값 반환
    };

    template <typename T> struct __empty__<T&> {
        static T& ret() {
            static T dummy;
            return dummy;
        }
    };

    template <> struct __empty__<void> {
        static void ret() {}
    };

    template <typename T, typename F> auto operator->*(T&& t, F&& f) {
        if constexpr(std::is_pointer_v<std::decay_t<T>>)
            return t ? f(*t) : __empty__<std::invoke_result_t<F, decltype(*t)>>::ret();
        else return f(std::forward<T>(t));
    }

    /// `to` is safe navigation feature of c++:
    /// usage:
    ///     as-is:
    ///     ```cpp
    ///         int getBrushColorCode(Resource r) {
    ///             auto& pallete = r.getPallete();
    ///             if(nul(pallete)) {
    ///                 log("pallete is null");
    ///                 return -1;
    ///             }
    ///
    ///             auto& canvas = pallete.getCanvas();
    ///             if(nul(canvas)) {
    ///                 log("canvas is null");
    ///                 return -1;
    ///             }
    ///
    ///             auto& brush = canvas.getBrush(BushType.SYSTEM);
    ///             if(nul(brush)) {
    ///                 log("brush is null");
    ///                 return -1;
    ///             }
    ///
    ///             return brush.getColorCode();
    ///         }
    ///     ```
    ///
    ///     to be:
    ///     ```cpp
    ///         int getBrushColorCode(Resource r) {
    ///             const int& code = r TO(getPallete()) TO(getCanvas()) TO(getBrush())
    ///             TO(getColorCode());
    ///             if(nul(code)) // if null returns during safe-navigation, output is nul
    ///             reference.
    ///                 return log("code is null"), -1;
    ///             return code;
    ///         }
    ///     ```
    /// but remember, namu doesn't create references that can be null.
    /// if you use TO on a reference that is null, it will behave UB.
    /// in many cases, the app will crash.
#define TO(fn)                                                                              \
    ->*[&](auto&& __p) -> std::decay_t<decltype(__p.fn)> {                                  \
        if constexpr(std::is_pointer_v<std::decay_t<decltype(__p)>>)                        \
            return !nul(__p) ? __p->fn : __empty__<std::decay_t<decltype(__p->fn)>>::ret(); \
        else return !nul(__p) ? __p.fn : __empty__<std::decay_t<decltype(__p.fn)>>::ret();  \
    }

} // namespace nm
