#pragma once

#include "dep.hpp"
#include "_nout.hpp"

namespace nm {
    class type;
    class typeProvidable;

    _nout strWrap __convert__(const typeProvidable& rhs);
    _nout strWrap __convert__(const type& rhs);
    _nout strWrap __convert__(const typeProvidable* rhs);
    _nout strWrap __convert__(const type* rhs);
}
