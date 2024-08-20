#pragma once

#include "srcSupply.hpp"

namespace nm {

    class _nout fileSupply: public srcSupply {
        NM(CLASS(fileSupply, srcSupply))

    public:
        fileSupply(const std::string& newPath);

    public:
        void* onSupplySrc(parser& ps, void* scanner) const override;

    private:
        std::string _path;
    };
}
