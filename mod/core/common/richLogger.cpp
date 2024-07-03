#include "richLogger.hpp"
#include "../builtin/primitive/nStr.hpp"
#include "../builtin/primitive/nInt.hpp"
#include "../visitor/visitInfo.hpp"

namespace nm {
    std::string convert(const arithmeticObj& it) { return !nul(it) ? it.as<nStr>()->get() : "null"; }
    std::string convert(const visitInfo& it) { return it.name; }
    std::string convert(const str& it) { return it ? it->getType().getName() : std::string("null"); }
    std::string convert(const src& it) { return it.getName(); }
}
