#include "richLog.hpp"

#include "../builtin/primitive/nInt.hpp"
#include "../builtin/primitive/nStr.hpp"
#include "../builtin/primitive/nVoid.hpp"
#include "../worker/visitor/visitInfo.hpp"

namespace nm {
    strWrap __convert__(const arithmeticObj& it) {
        return !nul(it) ? it.as<nStr>()->get() : "null";
    }

    strWrap __convert__(const str& it) {
        if(nul(it)) return strWrap("null");

        const std::string& name = it->getType().getName();
        const arithmeticObj& cast = it->cast<arithmeticObj>();
        if(nul(cast)) return name;
        if(cast.isSub<nVoid>()) return name;

        return strWrap(name + "(" + cast.as<nStr>()->get() + ")");
    }

    strWrap __convert__(const src& it) { return nul(it) ? "null" : it.getName(); }

    strWrap __convert__(const baseFunc& it) {
        return nul(it) ? "null" : it.getSrc().getName() + "(" + it.getParams().toStr() + ")";
    }

    strWrap __convert__(const param& it) {
        if(nul(it)) return strWrap("null");
        const node& org = it.getOrigin();
        return it.getName() + " " + (nul(org) ? "null" : org.getType().getName());
    }

    strWrap __convert__(const visitInfo& it) { return nul(it) ? "null" : it.name; }

    strWrap __convert__(const modifier& it) {
        return nul(it) ? "null" :
                         std::string("modifier(") + (it.isPublic() ? "public" : "protected") +
                (it.isExplicitOverride() ? std::string(", override)") : std::string(")"));
    }
}
