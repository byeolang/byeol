#pragma once

#include "indep/common.hpp"
#include "indep/def.hpp"
#include "indep/macro.hpp"

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
