#include "nextExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../visitor/visitor.hpp"

namespace nm {

    NAMU(DEF_ME(nextExpr), DEF_VISIT())

    str me::getEval() const {
        static nextRet inner;
        return inner;
    }
}
