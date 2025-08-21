/// @file
#pragma once

#include "leaf/common/dep.hpp"
#include "leaf/common/_nout.hpp"

namespace by {
    class leaf;
    _nout strWrap __convert__(const leaf* rhs);
    _nout strWrap __convert__(const leaf& rhs);
}
