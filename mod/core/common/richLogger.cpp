#include "richLogger.hpp"
#include "../builtin/primitive/nStr.hpp"
#include "../builtin/primitive/nInt.hpp"
#include "../visitor/visitInfo.hpp"

namespace nm {
    std::string convert(const nint& it) { return std::to_string(it); }
    std::string convert(const nflt& it) { return std::to_string(it); }
    std::string convert(const nchar& it) { return std::to_string(it); }
    std::string convert(const nbool& it) { return it ? "true" : "false"; }
    std::string convert(const std::string& it) { return it; }
    std::string convert(const nchar* it) { return std::string(it); }
    std::string convert(const ndbl& it) { return std::to_string(it); }

    std::string convert(const typeProvidable& it) { return it.getType().getName(); }
    std::string convert(const type& it) { return it.getName(); }

    std::string convert(const arithmeticObj& it) { return it.as<nStr>()->get(); }
    std::string convert(const visitInfo& it) { return it.name; }
    std::string convert(const str& it) { return it ? it->getType().getName() : std::string("null"); }
}
