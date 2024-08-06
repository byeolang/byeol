#include "ctor.hpp"
#include "visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(ctor))

    me::ctor(const params& ps, const blockExpr& blk): super(ps, nullptr, blk) {}

    str me::run(const args& a) {
        str ret = (node*) getRet()->clone();

        a.setMe(*ret);
        return super::run(a);
    }

    str me::getEval() const {
        return getRet();
    }
}
