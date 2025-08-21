/// @file
#pragma once

#include "core/worker/supply/srcSupply.hpp"

namespace by {
    /// @ingroup core
    /// @brief Buffer-based source supply
    /// @details Supplies source code from an in-memory string buffer to the parser.
    class _nout bufSupply: public srcSupply {
        BY(CLASS(bufSupply, srcSupply))

    public:
        bufSupply(const std::string& buf);

    public:
        void* onSupplySrc(parser& ps, void* scanner) const override;

    private:
        std::string _buf;
    };
}
