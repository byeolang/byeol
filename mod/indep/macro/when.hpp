#pragma once

#include "../def/_nout.hpp"

namespace nm {
    class _nout __dummy_class__ {
        __dummy_class__();

    public:
        static const __dummy_class__& get();

        void nothing() const;
    };

#define __WHEN__POSTFIX return ::nm::__dummy_class__::get()
#define WHEN(condition) \
    if(condition) __WHEN__POSTFIX
#define WHEN_NUL_1(v1) \
    if(nul(v1)) __WHEN__POSTFIX
#define WHEN_NUL_2(v1, v2) \
    if(nul(v1) || nul(v2)) __WHEN__POSTFIX
#define WHEN_NUL_3(v1, v2, v3) \
    if(nul(v1) || nul(v2) || nul(v3)) __WHEN__POSTFIX
#define WHEN_NUL_4(v1, v2, v3, v4) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4)) __WHEN__POSTFIX
#define WHEN_NUL_5(v1, v2, v3, v4, v5) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5)) __WHEN__POSTFIX
#define WHEN_NUL_6(v1, v2, v3, v4, v5, v6) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5) || nul(v6)) __WHEN__POSTFIX
#define WHEN_NUL(...) NM_OVERLOAD(WHEN_NUL, __VA_ARGS__)

} // namespace nm
