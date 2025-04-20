#include "leaf/common/richLog.hpp"

#include "leaf/ast/leaf.hpp"

namespace nm {
    strWrap __convert__(const leaf* rhs) { return rhs ? rhs->getName() : std::string("null"); }
}
