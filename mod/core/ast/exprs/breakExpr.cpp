#include "breakExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(breakExpr), DEF_VISIT())

    str me::getEval() const {
        static breakRet inner;
        return inner;
    }
}
