#include "macro.hpp"
#include "../type/ttype.hpp"
#include "../interface/typeProvidable.hpp"

namespace nm {
    std::string convert(const typeProvidable& it) { return !nul(it) ? it.getType().getName() : "null"; }
    std::string convert(const type& it) { return it.getName(); }
}
