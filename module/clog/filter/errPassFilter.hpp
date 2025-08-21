/// @file
#pragma once

#include "clog/filter/filterable.hpp"

namespace by {
    /// @ingroup clog
    /// @brief Filter that passes through error-level log messages
    /// @details Simple filter implementation that allows error-level messages
    /// to pass through while filtering out other levels.
    class _nout errPassFilter: public filterable {
        BY(ME(errPassFilter, filterable))

    public:
        nbool filt(errLv::level lv, const std::string& tag) const override;
    };
}
