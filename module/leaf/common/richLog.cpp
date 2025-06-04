#include "leaf/common/richLog.hpp"

#include "leaf/ast/leaf.hpp"

namespace nm {
    strWrap __convert__(const leaf* it) { return it ? __convert__(*it) : strWrap("null"); }

    strWrap __convert__(const leaf& it) { return it.getName(); }
}
