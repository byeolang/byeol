#include "FAOExpr.hpp"
#include "../../loader/interpreter/tverification.hpp"
#include "../../loader/interpreter/verification.inl"
#include "../../loader/interpreter/verifier.hpp"
#include "../../frame/thread.hpp"
#include "../../builtin/primitive.hpp"

namespace wrd {

    WRD_DEF_ME(FAOExpr)

    namespace {
        str primitives[] = {
            new wInt(),
            new wFlt(),
            new wStr(),
            new wChar(),
            new wBool(),
        };

        wbool checkEvalType(const node& eval) {
            if(nul(eval)) return false;
            for(str e : primitives)
                if(eval.isSub(*e))
                    return true;

            return false;
        }
    }

    const node& me::getEval() const {
        if(!_lhs || !_rhs) return str();

        return _lhs->getEval().reduce(_rhs->getEval());
    }

    WRD_VERIFY({
        WRD_DI("verify: FAOExpr: lhs & rhs should bind something.");

        const node& lhs = it.getLeft();
        const node& rhs = it.getRight();
        if(nul(lhs)) return _srcErr(errCode::LHS_IS_NULL);
        if(nul(rhs)) return _srcErr(errCode::RHS_IS_NULL);
        const node& lEval = lhs.getEval();
        const node& rEval = rhs.getEval();
        if(nul(lEval)) return _srcErr(errCode::LHS_IS_NULL);
        if(nul(rEval)) return _srcErr(errCode::RHS_IS_NULL);

        if(!checkEvalType(lEval)) return _srcErr(errCode::LHS_IS_NOT_ARITH, lEval->getType().getName().c_str());
        if(!checkEvalType(rEval)) return _srcErr(errCode::RHS_IS_NOT_ARITH, rEval->getType().getName().c_str());
    })

}
