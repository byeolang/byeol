#pragma once

#include "dep.hpp"

#ifdef WRD_BUILD_PLATFORM_IS_WINDOWS
// f***ing annoying another MSVC bug here:
//  first of all, I'm so sorry to start my slang. but I couldn't help spitting it out after
//  I used plenty hours of heading to the ground.
//
//  I don't know why, but unless define below variable here, I'll observe that the member-variable
//  '_subs' above was tried to be instantiated but failed.
//  error message said that 'You've used undefined type "identifiertstr<scopes>"'.
//  however, MSVC definately knows about tstr<T> and scopes types.
//
//  clang, gcc didn't complain about this.
namespace {
    static const inline scopes s1;
    static const inline packContainer* p1 = new tnmap<std::string, pack>();
}
#endif