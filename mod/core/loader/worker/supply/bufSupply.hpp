#pragma once

#include "srcSupply.hpp"

namespace nm {
    class _nout bufSupply : public srcSupply {
        NAMU(CLASS(bufSupply, srcSupply))

    public:
        bufSupply(const std::string& buf);

    public:
        void* onSupplySrc(parser& ps, void* scanner) const override;

    private:
        std::string _buf;
    };
}
