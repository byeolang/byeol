/// @file
#pragma once

#include "core/ast/src/src.hpp"

namespace by {
    /** @ingroup core
     *  @brief Dummy source location implementation
     *  @details Placeholder source location used when no actual source information is available.
     */
    class _nout dumSrc: public src {
        BY(CLASS(dumSrc, src))

    private:
        dumSrc();

    public:
        static me& singleton();
    };
}
