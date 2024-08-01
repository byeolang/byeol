#pragma once

#include "../def/_nout.hpp"

namespace nm {
    class _nout __dummy_class__ {
        __dummy_class__();

    public:
        static const __dummy_class__& get();

        inline void nothing() const {}
    };

#define __NM_WHEN__POSTFIX return ::nm::__dummy_class__::get()
#define NM_WHEN(condition) if(condition) __NM_WHEN__POSTFIX
#define NM_WHENNUL_1(v1) if(nul(v1)) __NM_WHEN__POSTFIX
#define NM_WHENNUL_2(v1, v2) if(nul(v1) || nul(v2)) __NM_WHEN__POSTFIX
#define NM_WHENNUL_3(v1, v2, v3) if(nul(v1) || nul(v2) || nul(v3)) __NM_WHEN__POSTFIX
#define NM_WHENNUL_4(v1, v2, v3, v4) if(nul(v1) || nul(v2) || nul(v3) || nul(v4)) __NM_WHEN__POSTFIX
#define NM_WHENNUL_5(v1, v2, v3, v4, v5) if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5)) __NM_WHEN__POSTFIX
#define NM_WHENNUL_6(v1, v2, v3, v4, v5, v6) if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5) || nul(v6)) __NM_WHEN__POSTFIX
#define NM_WHENNUL(...) NM_OVERLOAD(NM_WHENNUL, __VA_ARGS__)
}
