#pragma once

#include "dep.hpp"

// namuMeta macro's sub-commands:
//  CLONE:
//      defines normal clone func. it have to be defined if you
//      just have declared non abstract class.
#define __NM__DECL_CLONE(ME)                                   \
public:                                                        \
    clonable* clone() const override { return new ME(*this); } \
                                                               \
private:

//  TYPE:
//      defines super meta type.
//      for instance, if you set TYPE(type), then when you call getType() of your class,
//      it will returns instance of type of type.
#define __NM__DECL_TYPE(METATYPE)                                      \
public:                                                                \
    typedef METATYPE metaType;                                         \
    const ntype& getType() const override { return ttype<me>::get(); } \
                                                                       \
private:
