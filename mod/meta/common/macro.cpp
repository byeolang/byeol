#include "macro.hpp"
#include "../type/ttype.hpp"
#include "../interface/typeProvidable.hpp"

namespace nm {
    std::string convert(const typeProvidable& it) { return it.getType().getName(); }
    std::string convert(const type& it) { return it.getName(); }
}
