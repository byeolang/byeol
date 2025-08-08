#pragma once

#include "core/common/dep.hpp"

// INTERFACE:
//      marks that this class is abstract.
#define __BY__DECL_ADT_1(ME) \
    __BY__DECL_ME_1(ME)      \
    __BY__DECL_TYPE(ntype)   \
    __BY__DECL_INIT_META(ME)
#define __BY__DECL_ADT_2(ME, SUPER) __BY__DECL_ADT_3(ME, SUPER, ntype)
#define __BY__DECL_ADT_3(ME, SUPER, SUPERTYPE) \
    __BY__DECL_ME_2(ME, SUPER)                 \
    __BY__DECL_TYPE(SUPERTYPE)                 \
    __BY__DECL_INIT_META(ME)
#define __BY__DECL_ADT(...) BY_OVERLOAD(__BY__DECL_ADT, __VA_ARGS__)

// CLASS:
//      marks that this class is concrete class.
#define __BY__DECL_CLASS_1(ME) \
    __BY__DECL_ADT_1(ME)       \
    __BY__DECL_CLONE(ME)
#define __BY__DECL_CLASS_2(ME, SUPER) \
    __BY__DECL_ADT_2(ME, SUPER)       \
    __BY__DECL_CLONE(ME)
#define __BY__DECL_CLASS_3(ME, SUPER, SUPERTYPE) \
    __BY__DECL_ADT_3(ME, SUPER, SUPERTYPE)       \
    __BY__DECL_CLONE(ME)
#define __BY__DECL_CLASS(...) BY_OVERLOAD(__BY__DECL_CLASS, __VA_ARGS__)

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
