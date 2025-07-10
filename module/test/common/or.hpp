#pragma once

#include "test/common/dep.hpp"

#define OR_ASSERT(name) __OR_DO__(nul(__p)) FAIL() << #name << " is null";
