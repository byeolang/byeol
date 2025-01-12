#include "util.hpp"
#include "../ast/node.hpp"
#include "../ast/exprs/getExpr.hpp"

namespace nm {
    NM(DEF_ME(util))

    typeAttr me::checkTypeAttr(const std::string& name) {
        if(name.empty()) return ATTR_ERR;
        if(islower(name[0])) return ATTR_COMPLETE;
        if(name.size() == 1) return ATTR_CONST; // size==1 && first letter is uppercase.

        return std::all_of(name.begin(), name.end(), isupper) ? ATTR_CONST : ATTR_INCOMPLETE;
    }

    std::string me::getEvalTypeFrom(const node& value) {
        if(nul(value)) return "null";
        str eval = value.getEval();
        if(eval) return (eval->isComplete() ? "" : "@incomplete ") + eval->getType().getName();

        const auto& name = value.cast<getExpr>() THEN(getName());
        if(!nul(name)) return name;
        return value.getType().getName();
    }
}
