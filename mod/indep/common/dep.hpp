#pragma once

#include "../def/buildInformation.hpp"

#include <stdio.h>
#include <string.h> // for strerror
#include <sys/stat.h>

#include <iostream>
#include <string>
#ifndef NM_BUILD_PLATFORM_IS_WINDOWS
#    include <dirent.h> // not for winOS
#    include <unistd.h> // not for winOS
#else
// disable warning for MSVC
// TODO: fix the issue
#    pragma warning(disable: 4251)
#    pragma warning(disable: 4273)
#    pragma warning(disable: 4005)
#    pragma warning(disable: 4068)
#    pragma warning(disable: 4067)
#    pragma warning(disable: 4819)
#    pragma warning(disable: 4267)
#    pragma warning(disable: 4083)
#    pragma warning(disable: 5105)
#    pragma warning(disable: 4903)
#    pragma warning(disable: 4275)
#    pragma warning(disable: 4244)
#    pragma warning(disable: 4005)
#    pragma warning(disable: 4661)
#    pragma warning(disable: 4996)
#    pragma warning(disable: 4065)
#    pragma warning(disable: 4068)
#    pragma warning(disable: 4819)
#    pragma warning(disable: 4661)
#endif
#include <cstdint>
#include <string>