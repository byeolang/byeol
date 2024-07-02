#pragma once

#include "common.hpp"

namespace nm {
    class logLv {
        NAMU(ME(logLv))

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
