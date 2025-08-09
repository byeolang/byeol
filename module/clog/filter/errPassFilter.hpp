#pragma once

#include "clog/filter/filterable.hpp"

namespace by {
    class _nout errPassFilter: public filterable {
        BY(ME(errPassFilter, filterable))

    public:
        nbool filt(errLv::level lv, const std::string& tag) const override;
    };
}
