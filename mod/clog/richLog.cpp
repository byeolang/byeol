#include "richLog.hpp"

namespace nm {

    strWrap::strWrap(const std::string& rhs): data(rhs) {}

    const nchar* strWrap::unwrap() const { return data.c_str(); }

    noWrap<nint> __convert__(nint rhs) { return rhs; }
    noWrap<nuint> __convert__(nuint rhs) { return rhs; }
    noWrap<nflt> __convert__(nflt rhs) { return rhs; }
    noWrap<nchar> __convert__(nchar rhs) { return rhs; }
    noWrap<const nchar*> __convert__(const nchar* rhs) { return rhs; }
    noWrap<ndbl> __convert__(ndbl rhs) { return rhs; }
    noWrap<nint64> __convert__(nint64 rhs) { return rhs; }
    noWrap<nuint64> __convert__(nuint64 rhs) { return rhs; }
    strWrap __convert__(nbool rhs) { return strWrap(rhs ? "true" : "false"); }
    strWrap __convert__(const std::string& rhs) { return rhs; }
    strWrap __convert__(void* rhs) { return rhs ? platformAPI::toAddrId(rhs) : "null"; }
}
