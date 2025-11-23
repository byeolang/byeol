#include "stela/common/richLog.hpp"

#include "stela/ast/stela.hpp"

namespace by {
    strWrap __convert__(const stela* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const stela& it) { return it.getName(); }
}
