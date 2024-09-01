#pragma once

#include "filterable.hpp"

namespace nm {
    class _nout errPassFilter: public filterable {
        NM(ME(errPassFilter, filterable))

    public:
        nbool filt(logLv::level lv, const std::string& tag) const override;
    };
}
