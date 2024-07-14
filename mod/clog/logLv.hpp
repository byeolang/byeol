#pragma once

#include "common.hpp"

namespace nm {
    class _nout logLv {
        NM(ME(logLv))

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
