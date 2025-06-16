#pragma once

#include "indep/macro/to.hpp"

namespace nm {

    /// OR macro:
    ///     overview:
    ///         It enables `return ?:`, a type of safe navigation in modern languages, in C++.
    ///
    ///     prerequisites:
    ///         OR macro is based on `WHEN` macro. Before learning about OR, you need to know WHEN
    ///         first.
    ///
    ///     usage:
    ///         OR macro is used like this:
    ///             `<expr-evalution-as-pointer> OR.<when-expr>`
    ///
    ///         in this case, expr-evaluation-as-pointer must evaluate to a pointer type of a
    ///         certain type. if not, a compilation error occurs. an expression using the WHEN macro
    ///         is placed after OR, and is executed instead if the previous pointer was nullptr. if
    ///         the pointer was not nullptr, it is returned as a reference. as a result, using the
    ///         OR macro, you can safely perform nullcheck and handle non-null types.
    ///
    ///     example:
    ///         int* foo();
    ///
    ///         int main() {
    ///             // without OR:
    ///             int* value = foo();
    ///             if(!value) return -1;
    ///             return *value;
    ///
    ///             // with OR:
    ///             int& value = foo() OR.ret(-1) // If foo() returns nullptr, return -1.
    ///             return value;
    ///         }
    ///
    ///     the WHEN macro provides various methods in addition to simply returning an error.
    ///     please check it if want to know more.
    ///
    ///     FAQ:
    ///         Q. I want to use OR and also do casting as `(T&)`.
    ///         A. since OR's return type is reference, you may simply want to do it like below.
    ///
    ///             class A {};
    ///             class B : public A {};
    ///
    ///             A* foo() { return new B(); }
    ///
    ///             int main() {
    ///                 B& value = (A&) foo() OR.ret(-1); // <-- but a compile error occurs here.
    ///                 return 0;
    ///             }
    ///
    ///         the reason is that, as mentioned earlier, the left side of OR must always be a
    ///         pointer. therefore, it should not be `(T&)` but `(T*)`. this is correct codes.
    ///
    ///         int main() {
    ///             B& value = (B*) foo() OR.ret(-1); // or you may use `auto&`
    ///             return 0;
    ///         }
    struct __orStack__ {
        static void push(nbool val) { _stack.push_back(val); }

        static nbool pop() {
            nbool ret = _stack.back();
            _stack.pop_back();
            return ret;
        }

    private:
        inline static std::vector<nbool> _stack;
    };

    template <typename T, typename F> T& operator|(T* t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return *t;
    }

    template <typename T, typename F> const T& operator|(const T* t, F&& f) {
        f(t);
        // this may return null-reference but take it easy.
        // it'll never be used.
        return *t;
    }

#define __OR_DO__(_expr_)                             \
    | [&](auto&& __p) -> void {                       \
        __orStack__::push(nul(__p) ? _expr_ : false); \
    };                                                \
    if(__orStack__::pop())

#define OR_DO __OR_DO__(nul(__p))

#define __OR__(expr) __OR_DO__(expr) return NM_WHEN
#define OR __OR__(nul(__p))

#define OR_CONTINUE OR_DO continue
} // namespace nm
