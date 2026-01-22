/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    enum errCode {
        ERR_CODE_START = -1,
        NOT_SPECIFIED, // errCode.inl should starts from index 0.
                       // please refer nerr.cpp more detail.
#define PAIRX PAIRX_LEFT
#include "errCode.inl"
#undef PAIRX
        ERR_CODE_END,
    };
}
