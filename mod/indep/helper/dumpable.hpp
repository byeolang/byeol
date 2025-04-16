#pragma once

#include "../common.hpp"
#include "../def.hpp"

namespace nm {
    class _nout dumpable {
    public:
        virtual void dump() const = 0;
    };
}
