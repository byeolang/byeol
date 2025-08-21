/// @file
#pragma once

#include "core/worker/supply/srcSupply.hpp"

namespace by {

    class _nout fileSupply: public srcSupply {
        BY(CLASS(fileSupply, srcSupply))

    public:
        fileSupply(const std::string& newPath);

    public:
        void* onSupplySrc(parser& ps, void* scanner) const override;

    private:
        std::string _path;
    };
}
