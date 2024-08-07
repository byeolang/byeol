#pragma once

#include "../func.hpp"

namespace nm {
    class _nout ctor : public func {
        NM(CLASS(ctor, func), VISIT())

    public:
        ctor(const params& ps, const blockExpr& blk);

    public:
        str run(const args& a) override;
        str getEval() const override;
    };
}
