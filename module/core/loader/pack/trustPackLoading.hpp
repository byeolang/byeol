/// @file
#pragma once

#include "core/loader/pack/packLoading.hpp"

namespace by {

    class errReport;

    /// @ingroup core
    /// @brief Trusted pack loading implementation
    /// @details Pack loading strategy that skips verification for trusted sources.
    /// Used for loading system packs and other trusted code modules.
    class _nout trustPackLoading: public packLoading {
        BY(ADT(trustPackLoading, packLoading))

    public:
        nbool verify(errReport& rpt, obj& pak) override;
    };
}
