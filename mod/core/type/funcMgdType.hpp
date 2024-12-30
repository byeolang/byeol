#pragma once

#include "mgdType.hpp"

namespace nm {
    class _nout funcMgdType : public mgdType {
        NM(ME(funcMgdType, mgdType), CLONE(funcMgdType))

    protected:
        const ases& _getImpliAses() const override;
    };
}
