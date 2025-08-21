/// @file
#pragma once

#include "clog/common/dep.hpp"

namespace by {
    /// @ingroup clog
    /// @brief Interface for objects that can filter log messages
    /// @details Provides the basic interface for implementing custom log filters
    /// based on error level and tag criteria.
    class _nout filterable {
    public:
        virtual ~filterable() {}

    public:
        /// @return true means an error.
        virtual nbool filt(errLv::level lv, const std::string& tag) const = 0;
    };
}
