#include "nextExpr.hpp"

#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(nextExpr), DEF_VISIT())

    str me::getEval() const {
        static nextRet inner;
        return inner;
    }
}
