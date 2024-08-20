#include "retStateExpr.hpp"

#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(retStateExpr), DEF_VISIT())

    str me::run(const args& a) {
        thread::get()._getNowFrame().setRet(*getEval());
        return getEval();
    }

    priorType me::prioritize(const args& a) const { return NO_MATCH; }
}
