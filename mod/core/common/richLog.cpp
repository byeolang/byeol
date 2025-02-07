#include "richLog.hpp"

#include "../builtin/primitive/nInt.hpp"
#include "../builtin/primitive/nStr.hpp"
#include "../builtin/primitive/nVoid.hpp"
#include "../worker/visitor/visitInfo.hpp"

namespace nm {

    strWrap __convert__(const node& it) {
        if(nul(it)) return strWrap("null");
        const arithmeticObj& cast = it.cast<arithmeticObj>();
        if(!nul(cast)) return __convert__(cast);

        return __convert__((typeProvidable&) it);
    }

    strWrap __convert__(const str& it) {
        return __convert__(*it);
    }

    strWrap __convert__(const arithmeticObj& it) {
        const std::string& name = it.getType().getName();
        if(nul(it)) return strWrap("null");
        if(it.isSub<nVoid>()) return name;

        return strWrap(name + "(" + it.as<nStr>()->get() + ")");
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

    strWrap __convert__(const params& it) { return it.toStr(); }

    strWrap __convert__(const visitInfo& it) { return nul(it) ? "null" : it.name; }

    strWrap __convert__(const modifier& it) {
        return nul(it) ? "null" :
                         std::string("modifier(") + (it.isPublic() ? "public" : "protected") +
                (it.isExplicitOverride() ? std::string(", override)") : std::string(")"));
    }
}
