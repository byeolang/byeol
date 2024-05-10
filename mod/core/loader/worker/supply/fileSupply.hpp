#pragma once

#include "srcSupply.hpp"

namespace namu {

    class _nout fileSupply : public srcSupply {
        NAMU(CLASS(fileSupply, srcSupply))

    public:
        fileSupply(const std::string& newPath);

    public:
        void* onSupplySrc(parser& ps, void* scanner) const override;

    private:
        std::string _path;
    };
}
