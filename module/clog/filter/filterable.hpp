/// @file
#pragma once

#include "clog/common/dep.hpp"

namespace by {
    /** @ingroup clog
     *  @brief Interface for objects that can filter log messages
     *  @details Allows the @ref logger class to filter logging based on specific conditions.
     *  Provides the `filt()` function which receives log information as parameters. If this
     *  function returns true for a given log entry, that log will not be sent to the
     *  @ref stream.
     *
     *  Implement this interface to create custom log filtering logic based on error level
     *  and tag criteria.
     */
    class _nout filterable {
    public:
        virtual ~filterable() {}

    public:
        /**
         * @brief Determines whether a log entry should be filtered out.
         * @param lv The error level of the log entry.
         * @param tag The tag associated with the log entry.
         * @return true if the log entry should be filtered out (not sent to stream), false otherwise.
         */
        virtual nbool filt(errLv::level lv, const std::string& tag) const = 0;
    };
}
