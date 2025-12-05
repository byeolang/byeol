#include "core/ast/exprs/endExpr.hpp"
#include "core/builtin/scalar/nVoid.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {
    BY(DEF_ME(endExpr), DEF_VISIT())

    me::endExpr(const blockExpr& blk): super(blk) {}

    str me::infer() const {
        static nVoid inner;
        return inner;
    }
}
