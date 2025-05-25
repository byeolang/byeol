#include "clog/richLog.hpp"

namespace nm {

    strWrap::strWrap(const std::string& rhs): data(rhs) {}

    const nchar* strWrap::unwrap() const { return data.c_str(); }

    noWrap<nflt> __convert__(nflt rhs) { return rhs; }

    noWrap<nchar> __convert__(nchar rhs) { return rhs; }

    noWrap<const nchar*> __convert__(const nchar* rhs) { return rhs; }

    noWrap<ndbl> __convert__(ndbl rhs) { return rhs; }

    noWrap<nshort> __convert__(nshort rhs) { return rhs; }

    noWrap<nushort> __convert__(nushort rhs) { return rhs; }

    strWrap __convert__(nbool rhs) { return strWrap(rhs ? "true" : "false"); }

    strWrap __convert__(const std::string* rhs) {
        return nul(rhs) ? strWrap("null") : __convert__(*rhs);
    }

    strWrap __convert__(const std::string& rhs) { return rhs; }

    strWrap __convert__(void* rhs) { return rhs ? platformAPI::toAddrId(rhs) : "null"; }
} // namespace nm
