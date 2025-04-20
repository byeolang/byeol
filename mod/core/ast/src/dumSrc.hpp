#pragma once

#include "core/ast/src/src.hpp"

namespace nm {
    class _nout dumSrc: public src {
        NM(CLASS(dumSrc, src))

    private:
        dumSrc();

    public:
        static me& singleton();
    };
}
