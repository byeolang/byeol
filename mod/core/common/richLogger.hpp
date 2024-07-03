#pragma once

#include "dep.hpp"

namespace nm {
    class arithmeticObj;
    struct visitInfo;
    class node;
    typedef tstr<node> str;
    class src;

    std::string convert(const arithmeticObj& it);
    std::string convert(const visitInfo& it);
    std::string convert(const str& it);
    std::string convert(const src& it);
}
