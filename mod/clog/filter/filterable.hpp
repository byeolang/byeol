#pragma once

#include "../common/dep.hpp"
#include "../logLv.hpp"

namespace nm {
    class _nout filterable {
    public:
        virtual ~filterable() {}

    public:
        /// @return true means an error.
        virtual std::string filt(logLv::level lv, const nchar* tag, const std::string& msg) const = 0;
    };
}
