#pragma once

#include "indep/macro/to.hpp"

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

    template <typename T, typename F>
    T* operator|(T* t, F&& f) {
        if(!t) return nullptr;
        f(t);
        return t;
    }

    template <typename T, typename F>
    const T* operator|(const T* t, F&& f) {
        if(!t) return nullptr;
        f(t);
        return t;
    }

#define __OR_DO__(_expr_)                \
    | [&](auto&& __p) {                  \
        __orStack__::push(_expr_);       \
    };                                   \
    if(__orStack__::pop())

#define OR_DO __OR_DO__(nul(__p))

#define __OR__(expr) __OR_DO__(expr) return NM_WHEN
#define OR __OR__(nul(__p))

#define OR_CONTINUE OR_DO continue
} // namespace nm
