#include "core/ast/ctor/ctor.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/type/typeMaker.hpp"

namespace by {
    BY(DEF_ME(ctor), DEF_VISIT())

    me::ctor(const modifier& mod, const params& ps, const blockExpr& blk):
        super(mod, typeMaker::make<me>(baseObj::CTOR_NAME, ps, nullptr), blk) {}

    str me::run(const args& a) {
        super::run(a);
        return a.getMe();
    }

    str me::getEval() const { return getRet(); }

    const src& me::getSrc() const {
        static src inner(srcFile(), baseObj::CTOR_NAME, point{});
        return inner;
    }
}
