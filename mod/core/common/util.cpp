#include "util.hpp"

namespace nm {
    NM(DEF_ME(util))

    typeAttr me::checkTypeAttr(const std::string& name) {
        if(name.empty()) return NOTHING;
        if(islower(name[0])) return COMPLETE_OBJ;
        if(name.size() == 1) return NOTHING; // size==1 && first letter is uppercase.

        return std::all_of(name.begin(), name.end(), isupper) ? CONST : NOTHING;
    }
}
