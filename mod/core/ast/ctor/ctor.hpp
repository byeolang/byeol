#pragma once

#include "../func.hpp"

namespace nm {
    class _nout ctor: public func {
        NM(ME(ctor, func), INIT_META(ctor), CLONE(ctor), VISIT())
        typedef ntype metaType;

    public:
        ctor(const modifier& mod, const params& ps, const blockExpr& blk);

    public:
        str run(const args& a) override;
        str getEval() const override;
    };
}
