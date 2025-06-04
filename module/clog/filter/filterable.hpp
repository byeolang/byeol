#pragma once

#include "clog/common/dep.hpp"

namespace nm {
    class _nout filterable {
    public:
        virtual ~filterable() {}

    public:
        /// @return true means an error.
        virtual nbool filt(errLv::level lv, const std::string& tag) const = 0;
    };
}
