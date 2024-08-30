#include "richLog.hpp"

#include "../builtin/primitive/nInt.hpp"
#include "../builtin/primitive/nStr.hpp"
#include "../worker/visitor/visitInfo.hpp"

namespace nm {
    strWrap __convert__(const arithmeticObj& it) {
        return !nul(it) ? it.as<nStr>()->get() : "null";
    }

    strWrap __convert__(const str& it) { return it ? it->getType().getName() : "null"; }

    strWrap __convert__(const src& it) { return it.getName(); }

    strWrap __convert__(const visitInfo& it) { return it.name; }

    strWrap __convert__(const modifier& it) {
        return std::string("modifier(") +
            (it.isPublic() ? "public" : "protected") +
            (it.isExplicitOverride() ? std::string(", override)") : std::string(")"));
    }
}
