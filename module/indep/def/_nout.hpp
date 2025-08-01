/// @file
/// Platform-specific export/import declarations for Windows DLL support
#pragma once

#ifdef WIN32
#    undef _nout
#    ifdef __indep = 1 // I'm currently building indep module
#        define _nout __declspec(dllexport)
#    else
#        define _nout __declspec(dllimport)
#    endif
#else
#    undef _nout
#    define _nout
#endif