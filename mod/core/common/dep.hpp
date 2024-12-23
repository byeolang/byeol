#pragma once

#include <leaf.hpp>
#ifndef NM_BUILD_PLATFORM_IS_WINDOWS
#    include <dirent.h> // not compatible to winOs
#    include <dlfcn.h>
#else
#    include <windows.h> // for dll loading
#endif
#include <array>
