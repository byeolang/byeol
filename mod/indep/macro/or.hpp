#pragma once

#include "to.hpp"

namespace nm {

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

#define __OR_DO__(_expr_)                \
    ->*[&](auto& __p) -> decltype(__p) { \
        __orStack__::push(_expr_);       \
        return __p;                      \
    };                                   \
    if(__orStack__::pop())

#define OR_DO __OR_DO__(nul(__p))

#define __OR__(expr) __OR_DO__(expr) return NM_WHEN
#define OR __OR__(nul(__p))

#define OR_CONTINUE OR_DO continue
} // namespace nm
