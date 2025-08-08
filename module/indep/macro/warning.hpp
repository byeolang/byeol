#pragma once

#include "helper.hpp"

// clang-format off

#if defined(__clang__)
#    define BY_WARN_PUSH _Pragma("clang diagnostic push")
#    define BY_WARN_IGNORE(warn) _Pragma(BY_STR(clang diagnostic ignored #warn))
#    define BY_WARN_IGNORE_AUTOLOGICAL_COMPARE BY_WARN_IGNORE(-Wtautological-undefined-compare)
#    define BY_WARN_IGNORE_UNUSED_FUNCTION BY_WARN_IGNORE(-Wunused-function)
#    define BY_WARN_POP _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
#    define BY_WARN_PUSH _Pragma("GCC diagnostic push")
#    define BY_WARN_IGNORE(warn) _Pragma(BY_STR(GCC diagnostic ignored #warn))
#    define BY_WARN_IGNORE_AUTOLOGICAL_COMPARE
#    define BY_WARN_IGNORE_UNUSED_FUNCTION BY_WARN_IGNORE(-Wunused-function)
#    define BY_WARN_POP _Pragma("GCC diagnostic pop")
#else
#    define BY_WARN_PUSH
#    define BY_WARN_IGNORE(warn)
#    define BY_WARN_IGNORE_AUTOLOGICAL_COMPARE
#    define BY_WARN_IGNORE_UNUSED_FUNCTION
#    define BY_WARN_POP
#endif

// clang-format on
