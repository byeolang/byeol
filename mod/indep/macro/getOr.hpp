#pragma once

#include "then.hpp"

namespace nm {

    template <typename T> class _tget {
    public:
        static T set(T expr) { return inner = expr, get(); }
        static T get() { return inner; }

    private:
        inline static T inner; // NOLINT
    };

    template <typename T> class _tget<T*> {
    public:
        static T* set(const T* trg) { return *store() = (T*) trg; }

        static T* get() { return *store(); }

        static T** store() {
            static T* inner = 0; // NOLINT
            return &inner;
        }
    };

    template <typename T> class _tget<T&> {
    public:
        static T& set(const T& expr) { return *store() = (T*) &expr, get(); }

        static T& get() { return **store(); }

        static T** store() {
            static T* inner = 0; // NOLINT
            return &inner;
        }
    };

    struct __orRetStack__ {
        static void push(nbool val) { _stack.push_back(val); }

        static nbool pop() {
            nbool ret = _stack.back();
            _stack.pop_back();
            return ret;
        }

    private:
        inline static std::vector<nbool> _stack;
    };

    template <typename T> static nbool __isNul__(const T&& rhs) { return nul(rhs); }

    template <typename T> static nbool __isNul__(const T& rhs) { return nul(rhs); }

    template <typename T> static nbool __isNul__(const T* rhs) { return nul(rhs); }

#define _PUSH(exp) _tget<decltype(exp)>::set(exp)
#define _GET(exp) _tget<decltype(exp)>::get()
#define _NULR(exp) nulOf<decltype(exp)>()

    /* marble = 123 orRet false;
       marble = 123 ->* [&](.....) { ; } if(...) return false
       람다 안쪽에서 왼쪽 타입(int)에 대한 _PUSH를 진행.
       if 단에서 _GET(int)를 수행해서 처리.

       Q. 람다 안에서는 인자를 통해서 int라는 걸 알 수 있을 것이다.
       이 타입정보를 람다 밖으로 보낼 수도 있는가?
            생각을 좀 해보자. 일단 코드는 다음과 같이 된다고 가정하자.
            marble = <expr> orRet false;
            marble = <expr> ->* [&](auto& value) -> decltype(value) {
                return _PUSH(value);
            };
            if(__isNul__(_GET(???))) <-- decltype(value)
                return false;

       A1. 단순히 PUSH와 POP으로 stack으로 처리한다면??
           만약 <expr> 이 호출한 함수 안쪽에서 다시 PUSH POP이 있다고 가정해보자.
           그래도 PUSH -> POP 으로 이어진다면, 나중에 함수를 나왔어도 POP 한 것이 이어질 것이다.
    */

    // marble = getOr(123) orRet false;

    /*
       marble = 123 orRet(marble) false;
       marble = 123; if(__isNul__(a)) return false;
    */

#define __orRet__                        \
    ->*[&](auto& __p) -> decltype(__p) { \
        __orRetStack__::push(nul(__p)); \
        return __p;                      \
    };

#define orRet return
#define orNul(T) return nulOf<T>()
#define orDo
#define getOr(expr) \
    _PUSH(expr);    \
    if(__isNul__(_GET(expr)))

#define orRet1 orDo1 return
#define orNul1(T) orRet1 nulOf<T>()
#define orDo1 __orRet__ if(__orRetStack__::pop())

} // namespace nm
