#pragma once

#include "dep.hpp"

// INTERFACE:
//      marks that this class is abstract.
#define __NM__DECL_ADT_1(ME) \
        __NM__DECL_ME_1(ME) \
        __NM__DECL_TYPE(ntype) \
        __NM__DECL_INIT_META(ME)
#define __NM__DECL_ADT_2(ME, SUPER) \
        __NM__DECL_ADT_3(ME, SUPER, ntype)
#define __NM__DECL_ADT_3(ME, SUPER, SUPERTYPE) \
        __NM__DECL_ME_2(ME, SUPER) \
        __NM__DECL_TYPE(SUPERTYPE) \
        __NM__DECL_INIT_META(ME)
#define __NM__DECL_ADT(...) NM_OVERLOAD(__NM__DECL_ADT, __VA_ARGS__)

// CLASS:
//      marks that this class is concrete class.
#define __NM__DECL_CLASS_1(ME) \
        __NM__DECL_ADT_1(ME) \
        __NM__DECL_CLONE(ME)
#define __NM__DECL_CLASS_2(ME, SUPER) \
        __NM__DECL_ADT_2(ME, SUPER) \
        __NM__DECL_CLONE(ME)
#define __NM__DECL_CLASS_3(ME, SUPER, SUPERTYPE) \
        __NM__DECL_ADT_3(ME, SUPER, SUPERTYPE) \
        __NM__DECL_CLONE(ME)
#define __NM__DECL_CLASS(...) NM_OVERLOAD(__NM__DECL_CLASS, __VA_ARGS__)

// ACCEPT:
//      accept the visitor and let it can iterate sub elements.
//      please check 'visitor' class for more info.
#define __NM__DECL_VISIT_0() __NM__DECL_VISIT_1(super)
#define __NM__DECL_VISIT_1(SUPER) \
        public: \
            using SUPER::accept; \
            void accept(const visitInfo& i, visitor& v) override; \
        private:
#define __NM__DECL_VISIT(...) NM_OVERLOAD(__NM__DECL_VISIT, __VA_ARGS__)

#define __NM__DECL_DEF_VISIT_0() __NM__DECL_DEF_VISIT_1(me)
#define __NM__DECL_DEF_VISIT_1(ME) \
            void ME::accept(const visitInfo& i, visitor& v) { \
                v.visit(i, *this); \
            }
#define __NM__DECL_DEF_VISIT(...) NM_OVERLOAD(__NM__DECL_DEF_VISIT, __VA_ARGS__)
