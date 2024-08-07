#include "ctor.hpp"
#include "visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(ctor), DEF_VISIT())

    me::ctor(const params& ps, const blockExpr& blk): super(ps, nullptr, blk) {}

    str me::run(const args& a) {
        str ret = (node*) getRet()->clone();
        a.setMe(*ret);
        super::run(a);
        return ret;
    }

    str me::getEval() const { return getRet(); }
}
