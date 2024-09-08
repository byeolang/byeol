#include "endExpr.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(endExpr), DEF_VISIT())

    me::endExpr(const blockExpr& blk): super(blk) {}

    str me::getEval() const {
        static nVoid inner;
        return inner;
    }
}
