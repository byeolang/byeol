#pragma once

#include "blockExpr.hpp"

namespace nm {
    class _nout endExpr : public blockExpr {
        NM(CLASS(endExpr, blockExpr), VISIT())

    public:
        endExpr(const blockExpr& blk);

    public:
        str getEval() const override;
    };

    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<endExpr, strTactic> ends;
}
