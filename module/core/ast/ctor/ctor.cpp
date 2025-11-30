#include "core/ast/ctor/ctor.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/type/typeMaker.hpp"

namespace by {
    BY(DEF_ME(ctor), DEF_VISIT())

    me::ctor(const modifier& mod, const params& ps, const blockExpr& blk):
        super(mod, typeMaker::make<me>(ps, nullptr), blk) {}

    str me::run(const args& a) {
        super::run(a);
        return a.getMe();
    }

    str me::infer() const { return getRet(); }

    const src& me::getSrc() const {
        static src inner(srcFile(), ctor::CTOR_NAME, point{});
        return inner;
    }
}
