#include "richLog.hpp"

namespace nm {

    strWrap::strWrap(const std::string& rhs): data(rhs) {}

    const nchar* strWrap::unwrap() const { return data.c_str(); }

    noWrap<nint> __convert__(nint rhs) { return rhs; }
    noWrap<nuint> __convert__(nuint rhs) { return rhs; }
    noWrap<nflt> __convert__(nflt rhs) { return rhs; }
    noWrap<nchar> __convert__(nchar rhs) { return rhs; }
    strWrap __convert__(nbool rhs) {
        std::string d = rhs ? "true" : "false";
        strWrap ret(std::string("true"));
        return ret;
    }
    noWrap<const nchar*> __convert__(const nchar* rhs) { return rhs; }
    noWrap<ndbl> __convert__(ndbl rhs) { return rhs; }
    noWrap<nlong> __convert__(nlong rhs) { return rhs; }
    noWrap<nulong> __convert__(nulong rhs) { return rhs; }
    strWrap __convert__(const std::string& rhs) { return rhs; }
    strWrap __convert__(void* rhs) { return rhs ? platformAPI::toAddrId(rhs) : "null"; }
}
