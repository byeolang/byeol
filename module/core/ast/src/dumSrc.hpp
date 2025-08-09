#pragma once

#include "core/ast/src/src.hpp"

namespace by {
    class _nout dumSrc: public src {
        BY(CLASS(dumSrc, src))

    private:
        dumSrc();

    public:
        static me& singleton();
    };
}
