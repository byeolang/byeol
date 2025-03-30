#pragma once

#include "to.hpp"

namespace nm {

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

#define __orRet__                        \
    ->*[&](auto& __p) -> decltype(__p) { \
        __orRetStack__::push(nul(__p));  \
        return __p;                      \
    };

#define orRet orDo return
#define orNul(T) orRet nulOf<T>()
#define orDo __orRet__ if(__orRetStack__::pop())
#define orContinue orDo continue
#define orEx(...) orDo ::nm::exMaker::make(__VA_ARGS__)
} // namespace nm
