#include "ctor.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(ctor), DEF_VISIT())

    me::ctor(const modifier& mod, const params& ps, const blockExpr& blk):
        super(mod, ps, nullptr, blk) {}

    str me::run(const args& a) {
        super::run(a);
        return a.getMe();
    }

    str me::getEval() const { return getRet(); }
}
