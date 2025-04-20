#pragma once

#include "mgdType.hpp"

namespace nm {
    class _nout funcMgdType: public mgdType {
        NM(ME(funcMgdType, mgdType), CLONE(funcMgdType))

    public:
        funcMgdType(const std::string& name, const type& super, const params& ps, nbool isAdt,
            const node& ret);

    protected:
        const impliAses& _getImpliAses() const override;
    };
}
