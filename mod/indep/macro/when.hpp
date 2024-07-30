#pragma once

#include "../def/_nout.hpp"

namespace nm {
    class _nout __dummy_class__ {
        __dummy_class__();

    public:
        static const __dummy_class__& get();

        inline void nothing() const {}
    };

#define NM_WHEN(condition) \
    if(condition) return ::nm::__dummy_class__::get()
#define NM_WHENNUL(condition) \
    if(nul(condition)) return ::nm::__dummy_class__::get()
}
