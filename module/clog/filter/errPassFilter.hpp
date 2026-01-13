/// @file
#pragma once

#include "clog/filter/filterable.hpp"

namespace by {
    /** @ingroup clog
     *  @brief Filter that passes through error-level log messages only
     *  @details As the name implies, a @ref filterable class that only allows error-level logs
     *  to pass through. When this filter is registered with @ref logger, warning and info logs
     *  will not be output.
     *
     *  Not used directly; instead, create an instance and add it to logger.
     *
     *  @section Usage
     *  Example of using errPassFilter:
     *  @code
     *      const filters& prevFilters = logger::get().getFilters();
     *      filters fs(new errPassFilter());
     *      logger::get().setFilters(fs);
     *
     *      // Now only error-level logs will be output
     *
     *      logger::get().setFilters(prevFilters); // Restore previous filters
     *  @endcode
     */
    class _nout errPassFilter: public filterable {
        BY(ME(errPassFilter, filterable))

    public:
        nbool filt(errLv::level lv, const std::string& tag) const override;
    };
}
