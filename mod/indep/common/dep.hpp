#pragma once

#include <string>
#include <iostream>
#include <string.h> // for strerror
#include <sys/stat.h>
#include <stdio.h>
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
#	include <dirent.h> // not for winOS
#	include <unistd.h> // not for winOS
#endif
#include <string>
