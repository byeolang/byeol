#pragma once

#include "to.hpp"

namespace nm {

    struct __orDoStack__ {
        static void push(nbool val) { _stack.push_back(val); }

        static nbool pop() {
            nbool ret = _stack.back();
            _stack.pop_back();
            return ret;
        }

    private:
        inline static std::vector<nbool> _stack;
    };

#define __orDoCondition__(_expr_)        \
    ->*[&](auto& __p) -> decltype(__p) { \
        __orDoStack__::push(_expr_);     \
        return __p;                      \
    };

#define orDo __orDo__(nul(__p))
#define __orDo__(expr) __orDoCondition__(expr) if(__orDoStack__::pop())

#define orRet __orRet__(nul(__p))
#define __orRet__(expr) __orDo__(expr) return

#define orNul(T) orRet nulOf<T>()
#define orEmpty __orRet__(__p.isEmpty())
#define orContinue orDo continue
} // namespace nm
