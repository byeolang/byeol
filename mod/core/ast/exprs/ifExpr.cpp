#include "ifExpr.hpp"

#include "../../builtin/primitive/nBool.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "../../frame/frameInteract.hpp"
#include "../../visitor/visitor.hpp"
#include "retStateExpr.hpp"

namespace nm {

    NM(DEF_ME(ifExpr), DEF_VISIT())

    me::ifExpr(const node& exp, const blockExpr& thenBlk): _expr(exp), _then(thenBlk) {}

    me::ifExpr(const node& exp, const blockExpr& thenBlk, const blockExpr& elseBlk):
        _expr(exp), _then(thenBlk), _else(elseBlk) {}

    blockExpr& me::getThen() { return *_then; }

    blockExpr& me::getElse() { return *_else; }

    node& me::getCondition() { return *_expr; }

    str me::run(const args& a) {
        tstr<nBool> res = _expr->as<node>()->asImpli<nBool>();
        if(!res) return nVoid::singletone();

        nbool cond = res->cast<nbool>();
        NM_DI("%s ifExpr: condition[%s]", platformAPI::toAddrId(this), cond);
        auto& blk = cond ? *_then : *_else;
        if(!nul(blk)) {
            frameInteract f1(blk);
            return blk.run();
        }

        return str(nVoid::singletone());
    }

    str me::getEval() const {
        str thenEval = _then->getEval();
        if(!thenEval) return NM_E("thenEval is null"), thenEval;
        str elseEval = _else ? _else->getEval() : str();
        if(!elseEval) return NM_E("elseEval is null"), elseEval;

        if(thenEval->isSub<retStateExpr>())
            return NM_DI("thenEval is %s, accept elseEval[%s]", thenEval, elseEval), elseEval;
        if(elseEval->isSub<retStateExpr>())
            return NM_DI("elseEval is %s, accept thenEval[%s]", elseEval, thenEval), thenEval;

        // when you try to get eval from ifExpr, `then` and else block must be declared first.
        // if one of blocks has omitted, evaluation of ifExpr should be null.
        str ret = thenEval->deduce(*elseEval); // if elseEval is null, then thenEval only left.
        NM_DI("thenEval[%s] + elseVal[%s] -> %s", thenEval, elseEval, ret);
        return ret;
    }

    void me::setThen(const blockExpr& newThen) { _then.bind(newThen); }

    void me::setElse(const blockExpr& newElse) { _else.bind(newElse); }

    void me::setCondition(const node& newCondition) { _expr.bind(newCondition); }
} // namespace nm
