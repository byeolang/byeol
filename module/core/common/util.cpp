#include "core/common/util.hpp"
#include "core/ast/node.hpp"
#include "core/ast/exprs/getExpr.hpp"

namespace by {
    NM(DEF_ME(util))

    typeAttr me::checkTypeAttr(const std::string& name) {
        WHEN(name.empty()).ret(ATTR_ERR);
        WHEN(islower(name[0])).ret(ATTR_COMPLETE);
        WHEN(name.size() == 1).ret(ATTR_CONST); // size==1 && first letter is uppercase.

        return std::all_of(name.begin(), name.end(), isupper) ? ATTR_CONST : ATTR_INCOMPLETE;
    }

    std::string me::getEvalTypeFrom(const node& value) {
        str eval = value.getEval();
        WHEN(eval).ret((eval->isComplete() ? "" : "@incomplete ") + eval->getType().getName());

        getExpr& get = value.cast<getExpr>() OR.ret(value.getType().getName());
        return get.getName();
    }
}
