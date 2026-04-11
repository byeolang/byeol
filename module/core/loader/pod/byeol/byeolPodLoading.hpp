#pragma once

#include "core/loader/pod/trustPodLoading.hpp"

namespace by {

    /** @ingroup core
     *  @brief Byeol script pod loading implementation
     *  @details Loads pods written in the Byeol language.
     *  Uses the parser to convert source code into AST nodes.
     */
    class _nout byeolPodLoading: public trustPodLoading {
        BY(CLASS(byeolPodLoading, trustPodLoading))

    public:
        nbool parse(errReport& rpt, pod& pak) override;
        nbool expand(errReport& rpt, pod& pak) override;
        const std::string& getName() const override;
    };
}
