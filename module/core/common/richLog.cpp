#include "core/common/richLog.hpp"

#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/scalar/nVoid.hpp"
#include "core/worker/visitor/visitInfo.hpp"

namespace by {

    strWrap __convert__(const node* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const node& it) {
        const scalar* cast = it.cast<scalar>();
        WHEN(cast) .ret(__convert__(*cast));

        return __convert__((typeProvidable&) it);
    }

    strWrap __convert__(const str& it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const scalar* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const scalar& it) {
        const std::string& name = it.getType().getName();
        WHEN(it.isSub<nVoid>()) .ret(name);

        return strWrap(name + "(" + it.as<nStr>()->get() + ")");
    }

    strWrap __convert__(const src* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const src& it) { return it.getName(); }

    strWrap __convert__(const baseFunc* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const baseFunc& it) { return it.getSrc().getName() + "(" + it.getParams().toStr() + ")"; }

    strWrap __convert__(const param* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const param& it) {
        const node& org = it.getOrigin();
        return it.getName() + " " + org.getType().getName();
    }

    strWrap __convert__(const params* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const params& it) { return it.toStr(); }

    strWrap __convert__(const visitInfo* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const visitInfo& it) { return it.name; }

    strWrap __convert__(const modifier* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const modifier& it) {
        return std::string("modifier(") + (it.isPublic() ? "public" : "protected") +
            (it.isExplicitOverride() ? std::string(", override)") : std::string(")"));
    }
} // namespace by
