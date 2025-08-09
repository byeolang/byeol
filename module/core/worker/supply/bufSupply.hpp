#pragma once

#include "core/worker/supply/srcSupply.hpp"

namespace by {
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
