#pragma once

#include <stdio.h>
#include <string.h> // for strerror
#include <sys/stat.h>

#include <iostream>
#include <string>
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
#    include <dirent.h> // not for winOS
#    include <unistd.h> // not for winOS
#endif
#include <cstdint>
#include <string>
