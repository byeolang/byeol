#pragma once

#include "../common.hpp"
#include "../def.hpp"
#include "../macro.hpp"

namespace nm {
    class _nout errLv {
        NM(ME(errLv))

    public:
        enum level {
            ERR,
            WARN,
            INFO
        };

    public:
        static const std::string& getName(level lv);
    };
}
