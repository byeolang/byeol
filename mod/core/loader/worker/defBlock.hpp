#pragma once

#include "../../ast/scope.hpp"
#include "../../builtin/container/native/tnarr.hpp"

namespace nm {

    class _nout defBlock : public instance {
        NAMU(CLASS(defBlock, instance))

    public:
        defBlock();

    public:
        tstr<scope> asScope;
        tstr<narr> asPreCtor;
    };
}
