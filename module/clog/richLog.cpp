#include "clog/richLog.hpp"

namespace by {

    strWrap::strWrap(const std::string& rhs): data(rhs) {}

    const nchar* strWrap::unwrap() const { return data.c_str(); }

    noWrap<nflt> __convert__(nflt rhs) { return rhs; }

    noWrap<nflt> __convert__(nflt* rhs) { return rhs ? __convert__(*rhs) : 0.0f; }

    noWrap<nchar> __convert__(nchar rhs) { return rhs; }

    noWrap<nchar> __convert__(nchar* rhs) { return rhs ? __convert__(*rhs) : ' '; }

    noWrap<const nchar*> __convert__(const nchar* rhs) { return rhs; }

    noWrap<ndbl> __convert__(ndbl rhs) { return rhs; }

    noWrap<ndbl> __convert__(ndbl* rhs) { return rhs ? __convert__(*rhs) : 0.0; }

    noWrap<nshort> __convert__(nshort rhs) { return rhs; }

    noWrap<nshort> __convert__(nshort* rhs) { return rhs ? __convert__(*rhs) : 0; }

    noWrap<nushort> __convert__(nushort rhs) { return rhs; }

    noWrap<nushort> __convert__(nushort* rhs) { return rhs ? __convert__(*rhs) : 0; }

    strWrap __convert__(nbool rhs) { return strWrap(rhs ? "true" : "false"); }

    strWrap __convert__(nbool* rhs) { return rhs ? __convert__(*rhs) : strWrap("null"); }

    strWrap __convert__(const std::string* rhs) {
        return rhs ? __convert__(*rhs) : strWrap("null");
    }

    strWrap __convert__(const std::string& rhs) { return rhs; }

    strWrap __convert__(void* rhs) { return rhs ? platformAPI::toAddrId(rhs) : "null"; }
} // namespace by
