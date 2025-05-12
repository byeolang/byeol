#pragma once

#include "leaf/common/dep.hpp"
#include "leaf/common/_nout.hpp"

namespace nm {
    class leaf;
    _nout strWrap __convert__(const leaf* rhs);
    _nout strWrap __convert__(const leaf& rhs);
}
