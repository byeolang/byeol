#pragma once

#include "indep/macro/overload.hpp"
#include "indep/helper/typeTrait.hpp"
#include <vector>
#include <type_traits>

namespace nm {

    template <typename T, typename F> auto operator->*(T& t, F&& f) { return f(t); }

    template <typename T, typename F> auto operator->*(const T& t, F&& f) { return f(t); }

    template <typename T, typename F> auto operator->*(T* t, F&& f) {
        return t ? &f(*t) : typeTrait<std::decay_t<decltype(&f(*t))>>::ret();
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
        return __p.fn;                                                                 \
    }

} // namespace nm
