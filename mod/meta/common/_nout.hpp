#pragma once


#ifdef WIN32
#	pragma warning(disable : 4067)
#   undef _nout
#	ifdef __meta = 1
#        define _nout __declspec(dllexport)
#    else
#        define _nout __declspec(dllimport)
#    endif
#else
#    undef _nout
#    define _nout
#endif
