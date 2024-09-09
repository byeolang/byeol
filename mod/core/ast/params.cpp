#include "params.hpp"
#include "exprs/getExpr.hpp"

namespace nm {
    NM(DEF_ME(params))

    std::string me::toStr() const {
        int n = 0;
        std::string msg;
        for(const param& p: *this)
            msg += p.getName() + " " + util::getEvalTypeFrom(p.getOrigin()) +
                (++n >= len() ? "" : ",");
        return msg;
    }
}
