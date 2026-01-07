/// @file
#pragma once

#include "core/ast/exprs/blockExpr.hpp"

namespace by {
    /// @ingroup core
    /// @brief End keyword expression
    /// @details Implements the `end` keyword.
    ///
    /// @remark endExpr simply holds a block statement. @ref func executes the end.
    class _nout endExpr: public blockExpr {
        BY(CLASS(endExpr, blockExpr), VISIT())

    public:
        endExpr(const blockExpr& blk);

    public:
        str infer() const override;
    };

    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<endExpr, strTactic> ends;
}
