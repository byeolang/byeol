#pragma once

#include "core/type/ntype.hpp"

namespace by {
    class _nout baseFuncType : public ntype {
    protected:
        const ases& _getAses() const override;
    };
}
