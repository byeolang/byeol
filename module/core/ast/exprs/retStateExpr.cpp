#include "core/ast/exprs/retStateExpr.hpp"

#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/common/coreInternal.hpp"

namespace by {

    BY(DEF_ME(retStateExpr), DEF_VISIT())

    str me::eval(const args& a) {
        coreInternal::getNowFrame() TO(setRet(*infer()));
        return infer();
    }

    priorType me::prioritize(const args& a) const { return NO_MATCH; }
}
