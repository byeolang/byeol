#pragma once

#include "core/type/ntype.hpp"

namespace by {
    class _nout baseFuncType: public ntype {
        BY(ME(baseFuncType, ntype))

    protected:
        const impliAses& _getImpliAses() const override;
        const ases& _getAses() const override;
    };
}
