#pragma once

#include <swrd.hpp>
#include <functional>
#ifndef WRD_BUILD_PLATFORM_IS_WINDOWS
#   include <dirent.h> // not compatible to winOs
#   include <dlfcn.h>
#else
#   include <windows.h> // for dll loading
#endif
#include <array>
