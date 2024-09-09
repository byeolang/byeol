#include "util.hpp"

namespace nm {
    NM(DEF_ME(util))

    typeAttr me::checkTypeAttr(const std::string& name) {
        if(name.empty()) return NOTHING;
        if(islower(name[0])) return COMPLETE_OBJ;
        if(name.size() == 1) return NOTHING; // size==1 && first letter is uppercase.

        return std::all_of(name.begin(), name.end(), isupper) ? CONSTANT : NOTHING;
    }

    std::string me::getEvalTypeFrom(const node& value) {
        if(nul(value)) return "null";
        str eval = value.getEval();
        if(eval) return eval->getType().getName();

        const auto& name = safeGet(value.cast<getExpr>(), getName());
        if(!nul(name)) return name;
        return value.getType().getName();
    }
}
