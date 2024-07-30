#include "richLog.hpp"

#include "../ast/sobj.hpp"

namespace nm {
    strWrap __convert__(const sobj* rhs) { return rhs ? platformAPI::toAddrId(rhs) : "null"; }
}
