#include "core/ast/exprs/endExpr.hpp"
#include "core/builtin/primitive/nVoid.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {
    BY(DEF_ME(endExpr), DEF_VISIT())

    me::endExpr(const blockExpr& blk): super(blk) {}

    str me::getEval() const {
        static nVoid inner;
        return inner;
    }
}
