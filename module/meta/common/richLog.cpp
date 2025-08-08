#include "meta/common/richLog.hpp"
#include "meta/type/type.hpp"
#include "meta/interface/typeProvidable.hpp"

namespace by {

    strWrap __convert__(const type& rhs) { return rhs.getName(); }

    strWrap __convert__(const typeProvidable& rhs) { return rhs.getType().getName(); }

    strWrap __convert__(const type* rhs) { return rhs ? __convert__(*rhs) : strWrap("null"); }

    strWrap __convert__(const typeProvidable* rhs) {
        return rhs ? __convert__(*rhs) : strWrap("null");
    }
}
