#include "core/ast/exprs/breakExpr.hpp"

#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(breakExpr), DEF_VISIT())

    str me::getEval() const {
        static breakRet inner;
        return inner;
    }
}
