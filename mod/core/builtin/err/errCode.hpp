#pragma once

#include "../../common.hpp"

namespace nm {

    enum errCode {
        ERR_CODE_START = 0,
#define PAIRX PAIRX_LEFT
#include "errCode.inl"
#undef PAIRX
        ERR_CODE_END,
    };
}
