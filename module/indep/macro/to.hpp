#pragma once

#include "indep/macro/overload.hpp"
#include "indep/helper/typeTrait.hpp"
#include <vector>
#include <type_traits>

namespace nm {

    template <typename T, typename F> auto operator->*(T& t, F&& f) { return f(t); }

    template <typename T, typename F> auto operator->*(const T& t, F&& f) { return f(t); }

    template <typename T, typename F> auto operator->*(T* t, F&& f) {
        return t ? f(*t) : typeTrait<std::decay_t<decltype(f(*t))>>::ret();
    }

    /// `to` is safe navigation feature of c++:
    ///     TO supports the safe navigation features of modern languages very intuitively and naturally.
    ///     the basic usage is ` <pointer-like-variable> TO(yourAccessor())`.
    ///     let's explain with an example first before explain further.
    /// 
    /// usage:
    ///     let's assume that we have following classes.
    ///
    ///         struct Resource {
    ///             Pallete* getPallete(); // this can return nullptr.
    ///         };
    ///         struct Pallete {
    ///             Canvas* getCanvas(); // this can return nullptr.
    ///         };
    ///         struct Canvas {
    ///             Brush* getBrush(int type);
    ///         };
    /// 
    ///     as-is:
    ///         in language spec of c++, there is no sort of safe navigation thing, so we've to do like this.
    ///
    ///             int getBrushColorCode(Resource r) {
    ///                 auto* pallete = r.getPallete();
    ///                 if(!pallete) {
    ///                     log("pallete is null");
    ///                     return -1;
    ///                 }
    ///
    ///                 auto* canvas = pallete->getCanvas();
    ///                 if(!canvas) {
    ///                     log("canvas is null");
    ///                     return -1;
    ///                 }
    ///
    ///                 auto* brush = canvas->getBrush(BrushType.SYSTEM);
    ///                 if(nul(brush)) {
    ///                     log("brush is null");
    ///                     return -1;
    ///                 }
    ///
    ///                 return brush->getColorCode();
    ///             }
    ///
    ///         of course, this example illustrates a rather extreme train wreck pattern, and is a design that
    ///         should be avoided, but situations where you need to access a pointer to a certain number of
    ///         pointers occur frequently, and if you don't always check in advance whether the pointer is valid
    ///         every time you dereference it, UB will occur.
    ///
    ///     to be:
    ///         with safe navigation, whether you will receive nullptr or not is determined after the dereference
    ///         chain of all pointers is finished.
    ///         so the resulting code can become very concise.
    ///
    ///             int getBrushColorCode(Resource r) {
    ///                 int code = r TO(getPallete()) TO(getCanvas()) TO(getBrush(BrushType.SYSTEM)) TO(getColorCode());
    ///                 if(code == 0) { // if nullptr returns during safe-navigation, output is default value.
    ///                     log("code is null")
    ///                     return -1;
    ///                 }
    ///                 return code;
    ///             }
    ///
    /// note:
    ///     as you can see, it looks easy to use, but there are a few things to be aware of.
    ///
    ///         1. the previously mentioned pointer-like-variable does not simply mean pointers,
    ///            but also includes classes that satisfy the following conditions.
    ///
    ///             a. a class that defines operator->().
    ///             b. a class that defines operator*().
    ///             c. a class that defines operator bool().
    ///
    ///            you may have noticed that the classes that satisfy the above conditions are usually smart
    ///            pointers like `unique_ptr`.
    ///            the byeol repository provides separate smart pointers and classes that replace std::optional<T>
    ///            for API consistency and safe type checking.
    ///             e.g. tstr<T>, tweak<T>, binder, tmay<T>, tres<T> are included here.
    ///
    ///            the TO() macro is designed to work properly even if the return value of the function is a
    ///            value or reference to the above class.
    ///            it works even if it exists in the middle of safe navigation chain. for example,
    ///
    ///                 struct Resource {
    ///                     Pallete* getPallete(); // this can return nullptr.
    ///                 };
    ///                 struct Pallete {
    ///                     Canvas& getCanvas(); // this *never* returns nullptr.
    ///                 };
    ///                 struct Canvas {
    ///                     tstr<Brush> getBrush(int type); // this is not pointer, but pointer-like-variable.
    ///                 };
    ///
    ///                 int getBrushColorCode(Resource r) {
    ///                     // however you can do almost same like above example.
    ///                     int code = r TO(getPallete()) TO(getCanvas().getBrush(BrushType.SYSTEM)) TO(getColorCode())
    ///                     if(code == 0) return -1;
    ///                     return code;
    ///                 }
    ///
    ///         2. if nullptr returned during the chain, the final result value becomes the default value.
    ///            if it is T*, it will be nullptr, but if it is T, i.e. a function that returns by value,
    ///            the return value will be T{}.
    ///
    ///         3. never put a reference in TO().
    ///            references are always non-null, so you can access them directly.
    ///
    ///         4. it is not recommended for any function to return a pointer type to a pointer-like-variable.
    ///            pointer-like-variable is a sufficiently lightweight class. You can return it by value,
    ///            or if you don't like that, return it by reference.
    ///
    ///         5. it goes very well with OR macro.
    ///            please check the usage of OR macro in advance. If you also use WHEN macro, the code
    ///            will become more concise.
    ///
    ///            int getBrushColorMode(Resource r) {
    ///                // uses OR macro. so type of `brush` is not `Brush*`.
    ///                auto& brush = r TO(getPallete()) TO(getCanvas().getBrush(BrushType.SYSTEM))
    ///                                OR.err("code is null").ret(-1)
    ///                return brush.getColorCode();
    ///            }

#define TO(fn) ->*[&](auto&& __p) -> std::decay_t<decltype(__p.fn)> { return __p.fn; }

} // namespace nm
