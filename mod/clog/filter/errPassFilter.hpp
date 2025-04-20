#pragma once

#include "clog/filter/filterable.hpp"

namespace nm {
    class _nout errPassFilter: public filterable {
        NM(ME(errPassFilter, filterable))

    public:
        nbool filt(errLv::level lv, const std::string& tag) const override;
    };
}
