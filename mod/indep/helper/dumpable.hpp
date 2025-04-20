#pragma once

#include "indep/common.hpp"
#include "indep/def.hpp"

namespace nm {
    class _nout dumpable {
    public:
        virtual void dump() const = 0;
    };
}
