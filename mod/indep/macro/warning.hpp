#pragma once

#include "helper.hpp"

#if defined(__clang__)
#    define NM_WARN_PUSH _Pragma("clang diagnostic push")
#    define NM_WARN_IGNORE(warn) _Pragma(NM_STR(clang diagnostic ignored #warn))
#    define NM_WARN_IGNORE_AUTOLOGICAL_COMPARE NM_WARN_IGNORE(-Wtautological - undefined - compare)
#    define NM_WARN_IGNORE_UNUSED_FUNCTION NM_WARN_IGNORE(-Wunused - function)
#    define NM_WARN_POP _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
#    define NM_WARN_PUSH _Pragma("GCC diagnostic push")
#    define NM_WARN_IGNORE(warn) _Pragma(NM_STR(GCC diagnostic ignored #warn))
#    define NM_WARN_IGNORE_AUTOLOGICAL_COMPARE
#    define NM_WARN_IGNORE_UNUSED_FUNCTION NM_WARN_IGNORE(-Wunused - function)
#    define NM_WARN_POP _Pragma("GCC diagnostic pop")
#else
#    define NM_WARN_PUSH
#    define NM_WARN_IGNORE(warn)
#    define NM_WARN_IGNORE_AUTOLOGICAL_COMPARE
#    define NM_WARN_IGNORE_UNUSED_FUNCTION
#    define NM_WARN_POP
#endif
