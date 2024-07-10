#pragma once

#ifdef WIN32
#	undef _nout
#	ifdef __meta = 1 // I'm currently building module
#		define _nout __declspec(dllexport)
#	else
#		define _nout __declspec(dllimport)
#	endif
#else
#	undef _nout
#	define _nout
#endif
