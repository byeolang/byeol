#pragma once

#include <core.hpp>

#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
// another another another MSVC's unexpected error here:
//  first of all, I'm so sorry to start my slang. but I couldn't help spitting it out after
//  I used plenty hours of heading to the ground.
//
//  I don't know why, but unless define below variable here, I'll observe that the member-variable
//  '_subs' above was tried to be instantiated but failed.
//  error message said that 'You've used undefined type "identifiertstr<scope>"'.
//  however, MSVC definately knows about tstr<T> and scope types.
//
//  clang, gcc didn't complain about this.
namespace nm {
	static const inline nmap inner;
}
#endif