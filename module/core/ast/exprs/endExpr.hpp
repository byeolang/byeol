/// @file
#pragma once

#include "core/ast/exprs/blockExpr.hpp"

namespace by {
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
