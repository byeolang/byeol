/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/def.hpp"
#include "indep/macro.hpp"

namespace by {
    /** @ingroup indep
     *  @brief Error level definitions and utilities
     *  @details Provides error severity levels and corresponding name lookup.
     */
    class _nout errLv {
        BY(ME(errLv))

    public:
        enum level {
            ERR,
            WARN,
            INFO
        };

    public:
        /**
         *  @brief Get human-readable name for error level
         *  @param lv Error level to convert to string
         */
        static const std::string& getName(level lv);
    };
}
