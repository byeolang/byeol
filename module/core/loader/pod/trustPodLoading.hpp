/// @file
#pragma once

#include "core/loader/pod/podLoading.hpp"

namespace by {

    class errReport;

    /** @ingroup core
     *  @brief Trusted pod loading implementation
     *  @details Pod loading strategy that skips verification for trusted sources.
     *  Used for loading system pods and other trusted code modules.
     */
    class _nout trustPodLoading: public podLoading {
        BY(ADT(trustPodLoading, podLoading))

    public:
        nbool expand(errReport& rpt, pod& pak) override;
        nbool verify(errReport& rpt, pod& pak) override;
    };
}
