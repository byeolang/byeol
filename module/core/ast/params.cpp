#include "core/ast/params.hpp"
#include "core/ast/exprs/getExpr.hpp"

namespace nm {
    NM(DEF_ME(params))

    nbool me::operator==(const me& rhs) const {
        WHEN(len() != rhs.len()).ret(false);

        for(nidx n = 0; n < len(); n++)
            WHEN(*get(n) != rhs[n]).ret(false);
        return true;
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    std::string me::toStr() const {
        int n = 0;
        std::string msg;
        for(const param& p: *this)
            msg += p.getName() + " " + util::getEvalTypeFrom(p.getOrigin()) +
                (++n >= len() ? "" : ",");
        return msg;
    }

    me me::make(const strings& names, const narr& args) {
        WHEN(names.size() != args.len())
            .err("names.len[%s] != args.len[%s]", names.size(), args.len())
            .ret(me());

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
} // namespace nm
