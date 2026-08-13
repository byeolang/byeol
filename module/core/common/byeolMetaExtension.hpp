/// @file
#pragma once

#include "core/common/dep.hpp"

// overriding base type of `getType()`:
#ifdef __BY__BASE_TYPE
#   undef __BY__BASE_TYPE
#   define __BY__BASE_TYPE ntype
#endif

// VISIT:
//  override `stela`'s macro.
#ifdef __BY__DECL_VISIT_1
#   undef __BY__DECL_VISIT_1
#   define __BY__DECL_VISIT_1(SUPER)                             \
    public:                                                   \
        using SUPER::accept;                                  \
        void accept(const visitInfo& i, visitor& v) override; \
    private:
#endif

#ifdef __BY__DECL_DEF_VISIT_1
#   undef __BY__DECL_DEF_VISIT_1
#   define __BY__DECL_DEF_VISIT_1(ME) \
    void ME::accept(const visitInfo& i, visitor& v) { v.visit(i, *this); }
#endif
