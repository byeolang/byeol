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

    me me::make(const strings& names, const narr& args) {
        if(names.size() != args.len()) {
            NM_E("names.len[%s] != args.len[%s]", names.size(), args.len());
            return me();
        }

        me ret;
        for(nidx n = 0; n < names.size(); n++)
            ret.add(new param(names[n], args[n]));
        return ret;
    }

    me me::make(const narr& args) {
        strings names;
        for(nidx n = 0; n < args.len(); n++)
            names.push_back(std::to_string(n));
        return make(names, args);
    }
}
