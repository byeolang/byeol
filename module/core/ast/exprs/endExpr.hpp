/// @file
#pragma once

#include "core/ast/exprs/blockExpr.hpp"

namespace by {
    /// @ingroup core
    /// @brief End @ref blockExpr for function cleanup
    /// @details Represents end blocks that execute after function completion.
    /// Used for cleanup operations and finally-like behavior in functions.
    class _nout endExpr: public blockExpr {
        BY(CLASS(endExpr, blockExpr), VISIT())

    public:
        endExpr(const blockExpr& blk);

    public:
        str getEval() const override;
    };

    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<endExpr, strTactic> ends;
}
