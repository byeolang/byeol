/// @file
#pragma once

#include "core/common/dep.hpp"

// overriding base type of `getType()`:
#ifdef __BY__BASE_TYPE
#   undef __BY__BASE_TYPE
#   define __BY__BASE_TYPE ntype
#endif

// ACCEPT:
//      accept the visitor and let it can iterate sub elements.
//      please check 'visitor' class for more info.
#define __BY__DECL_VISIT_0() __BY__DECL_VISIT_1(super)
#define __BY__DECL_VISIT_1(SUPER)                         \
public:                                                   \
    using SUPER::accept;                                  \
    void accept(const visitInfo& i, visitor& v) override; \
                                                          \
private:
#define __BY__DECL_VISIT(...) BY_OVERLOAD(__BY__DECL_VISIT, __VA_ARGS__)

#define __BY__DECL_DEF_VISIT_0() __BY__DECL_DEF_VISIT_1(me)
#define __BY__DECL_DEF_VISIT_1(ME) \
    void ME::accept(const visitInfo& i, visitor& v) { v.visit(i, *this); }
#define __BY__DECL_DEF_VISIT(...) BY_OVERLOAD(__BY__DECL_DEF_VISIT, __VA_ARGS__)
