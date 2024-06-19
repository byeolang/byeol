#include "whileExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../../frame/thread.hpp"
#include "../../frame/frameInteract.hpp"

namespace namu {

    NAMU(DEF_ME(whileExpr), DEF_VISIT())

    me::whileExpr(const node& condition, const blockExpr& blk): super(blk), _condition(condition),
        _initEval(false) {}

    node& me::getCondition() {
        return *_condition;
    }

    str me::run(const args& a) {
        blockExpr& blk = getBlock();
        if(!_condition) return NAMU_E("_condition is null."), str();
        if(nul(blk)) return NAMU_E("blk is null."), str();

        arr& ret = _preprocess();
        while(true) {
            str ased = _condition->asImpli<nBool>();
            if(!ased) return NAMU_E("cast to bool has been failed."), str();
            if(!ased->cast<nbool>()) break;

            frameInteract f1(blk); {
                ret.add(*blk.run());
                if(_postprocess())
                    return ret;
            }
        }

        return ret;
    }

    str me::getEval() const {
        if(_initEval) return super::getEval();

        _initEval = true;
        arr& newEval = *new arr(*getBlock().getEval()); // elem of last stmt.
        setEval(newEval);
        return newEval;
    }
}
