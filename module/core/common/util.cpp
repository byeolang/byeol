#include "core/common/util.hpp"
#include "core/ast/node.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/worker/supply/fileSupply.hpp"
#include <regex>

namespace by {
    BY(DEF_ME(util))

    typeAttr me::checkTypeAttr(const std::string& name) {
        WHEN(name.empty()) .ret(ATTR_ERR);
        WHEN(islower(name[0])) .ret(ATTR_COMPLETE);
        WHEN(name.size() == 1) .ret(ATTR_CONST); // size==1 && first letter is uppercase.

        return std::all_of(name.begin(), name.end(), isupper) ? ATTR_CONST : ATTR_INCOMPLETE;
    }

    std::string me::getEvalTypeFrom(const node& value) {
        str eval = value.infer();
        WHEN(eval) .ret((eval->isComplete() ? "" : "@incomplete ") + eval->getType().getName());

        getExpr& get = value.cast<getExpr>() OR.ret(value.getType().getName());
        return get.getName();
    }

    std::string me::joinVectorString(const std::vector<std::string>& container) {
        std::string ret;
        for(ncnt n = 0; n < container.size(); n++) {
            ret += container[n];
            if(n > 0) ret += ", ";
        }
        return ret;
    }

    std::string me::joinVectorString(const tucontainable<nStr, nStr*, nStr&>& container) {
        std::string ret;
        nbool isFirst = true;
        for(auto e = container.begin(); e; e++) {
            ret += e->get();

            if(!isFirst) ret += ", ";
            isFirst = true;
        }
        return ret;
    }
} // namespace by
