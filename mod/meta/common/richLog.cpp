#include "richLog.hpp"
#include "type/type.hpp"
#include "../interface/typeProvidable.hpp"

namespace nm {
    strWrap __convert__(const type& rhs) { return !nul(rhs) ? rhs.getName() : "null"; }

    strWrap __convert__(const typeProvidable& rhs) {
        return !nul(rhs) ? rhs.getType().getName() : "null";
    }
}
