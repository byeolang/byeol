#include "core/ast/exprs/nextExpr.hpp"

#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(nextExpr), DEF_VISIT())

    str me::infer() const {
        static nextRet inner;
        return inner;
    }
}
