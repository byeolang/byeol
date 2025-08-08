#include "core/ast/exprs/retStateExpr.hpp"

#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    NM(DEF_ME(retStateExpr), DEF_VISIT())

    str me::run(const args& a) {
        thread::get()._getNowFrame() TO(setRet(*getEval()));
        return getEval();
    }

    priorType me::prioritize(const args& a) const { return NO_MATCH; }
}
