#pragma once

#include "core/loader/pack/trustPackLoading.hpp"

namespace by {

    /** @ingroup core
     *  @brief Byeol script pack loading implementation
     *  @details Loads packs written in the Byeol language.
     *  Uses the parser to convert source code into AST nodes.
     */
    class _nout byeolPackLoading : public trustPackLoading {
        BY(CLASS(byeolPackLoading, trustPackLoading))

    public:
        nbool parse(errReport& rpt, pack& pak) override;
        const std::string& getName() const override;
    };
}
