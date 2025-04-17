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

#define __OR_DO__(_expr_)        \
    ->*[&](auto& __p) -> decltype(__p) { \
        __orDoStack__::push(_expr_);     \
        return __p;                      \
    }; \
    if(__orDoStack__::pop())

#define OR_DO __OR_DO__(nul(__p))

#define OR_RET __OR_RET__(nul(__p))
#define __OR_RET__(expr) __OR_DO__(expr) return

#define OR_NUL(T) OR_RET nulOf<T>()
#define OR_CONTINUE OR_DO continue
} // namespace nm
