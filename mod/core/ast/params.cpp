#include "params.hpp"

namespace nm {
    NM(DEF_ME(params))

    std::string me::toStr() const {
        int n=0;
        std::string msg;
        for(const param& p : *this)
            msg += p.getName() + " " + p.getOrigin().getType().getName() + (++n >= len() ? "" : ",");
        return msg;
    }
}
