#include "util.hpp"

namespace nm {
    NM(DEF_ME(util))

    typeAttr me::_checkTypeAttrWith(const std::string& name) {
        if(name.empty()) return NOTHING;

        nchar first = name[0];
        if(islower(first))
            return COMPLETE_OBJ;
        return std::all_of(name.begin(), name.end(), isupper) ? CONST : NOTHING;
    }
}
