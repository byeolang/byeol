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
    ///             // now, you don't need to dereference `value` everytime!
    ///             return value;
    ///         }
    ///
    ///     the WHEN macro provides various methods in addition to simply returning an error.
    ///     please check it if want to know more.
    ///
    ///     FAQ:
    ///         Q. I want to use OR and also do casting as `(T&)`.
    ///         A. since OR's return type is reference, you may simply write some code like below.
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
    ///
    ///         the reason is that, as mentioned earlier, the left side of OR must always be a
    ///         pointer. therefore, it should not be `(T&)` but `(T*)`. this is correct codes.
    ///
    ///         int main() {
    ///             B& value = (B*) foo() OR.ret(-1); // or you may use `auto&`
    ///             return 0;
    ///         }
    ///
    ///         if you want to cast the final result of a TO() chain to T&, you must surround the
    ///         entire TO() chain with parentheses.
    ///
    ///             B& value = (B*) (foo() TO(getA()) TO(getMayB())) OR.ret();
    ///
    ///
    ///
    ///         Q. Can I use OR after `return`?
    ///         A. `OR` was created based on the precondition that it would be used when defining a variable.
    ///         It cannot be used with the `return` keyword.
    ///
    ///
    ///         Q. I used OR macro with auto& and got `Non-const lvalue reference to...` error.
    ///            my code is like below,
    ///
    ///             auto& ret = _sub[name].get() OR.ret();
    ///
    ///         A. don't use auto keyword.
    ///         the actual return type of `OR` macro could be `tmedium<T>` or `tstr` or `tweak`.
    ///         it differs in context which you're using. it was specified in each operand class file,
    ///         for instance, 'tstr.hpp'. and if you used OR macro which returns `tmedium`, it is implicitly
    ///         returned as T& through this class.
    ///         therefore, unless you have a special situation where you want to use tmedium, specify the type
    ///         directly instead of auto.
    ///
    ///             MyClass& ret = _sub[name].get() OR.ret();
    ///             // or,
    ///             tstr<MyClass> ret = youGetThisInstanceOnHeap() OR.ret();
    ///
    ///
    ///         Q. I used OR in a function whose return type is tmay and initialized it with a T&& variable,
    ///         but the value is strange.
    ///
    ///             tmay<A> foo();
    ///             A&& a = foo() OR.ret();
    ///
    ///         A. TLDR; take rvalue with type `tmay<A>&&` just like rvalue reference to return type of the function.
    ///
    ///             tmay<A>&& a = foo();
    ///
    ///         reason:
    ///             foo() returns tmay by value. If you receive something returned by value as an rvalue reference,
    ///             its life would be extended, but in this case, since it is not received as tmay<A>, the value
    ///             inside it is taken out and returned, so the tmay temporary object does not extend its life and
    ///             starts to die immediately.
    ///             as a result, a garbage value is bound to `A&& a`.

#define __OR_DO__(_expr_)                             \
    | [&](auto&& __p) -> void {                       \
        __orStack__::push(nul(__p) ? _expr_ : false); \
    };                                                \
    if(__orStack__::pop())

#define OR_DO __OR_DO__(nul(__p))

#define __OR__(expr) __OR_DO__(expr) return NM_WHEN
#define OR __OR__(nul(__p))
#define OR_RET_CTOR __OR_DO__(nul(__p)) return

#define OR_CONTINUE OR_DO continue

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

} // namespace nm
