#pragma once

#include "../ast/scope.hpp"
#include "../ast/slot.hpp"

#ifdef NM_BUILD_PLATFORM_IS_WINDOWS

// f***ing annoying another MSVC bug here:
//  MSVC is not able to handle `dllexport` for class templates if it's not defined variable at least
//  once. c++ standards note that supports this functionality, but MSVC couldn't do it yet even
//  still 2024. so I have to let include this file for all every `cpp` files using scope or tnmap.
//  they are class templates to be dllexported.
//
//  however clang, gcc supports the feature so they didn't complain about this.
namespace nm {
    namespace {
        static const inline scope s1;
        static const inline slotContainer* p1 = new tnmap<std::string, slot>();
    }
}
#endif
