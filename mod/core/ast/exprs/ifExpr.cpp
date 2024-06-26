#include "ifExpr.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "../../visitor/visitor.hpp"
#include "../../builtin/primitive/nBool.hpp"
#include "../../frame/frameInteract.hpp"
#include "retStateExpr.hpp"

namespace namu {

    NAMU(DEF_ME(ifExpr), DEF_VISIT())

    me::ifExpr(const node& exp, const blockExpr& thenBlk): _expr(exp), _thenBlk(thenBlk) {}
    me::ifExpr(const node& exp, const blockExpr& thenBlk, const blockExpr& elseBlk):
        _expr(exp), _thenBlk(thenBlk), _elseBlk(elseBlk) {}

    blockExpr& me::getThenBlk() {
        return *_thenBlk;
    }

    blockExpr& me::getElseBlk() {
        return *_elseBlk;
    }

    node& me::getCondition() {
        return *_expr;
    }

    str me::run(const args& a) {
        tstr<nBool> res = _expr->as<node>()->asImpli<nBool>();
        if(!res) return nVoid::singletone();

        nbool cond = res->cast<nbool>();
        NAMU_DI("ifExpr: condition[%s]", cond ? "true" : "false");
        if(cond) {
            frameInteract f1(*_thenBlk); {
                return _thenBlk->run();
            }
        } else if(_elseBlk) {
            frameInteract f2(*_elseBlk); {
                return _elseBlk->run();
            }
        }

        return str(nVoid::singletone());
    }

    str me::getEval() const {
        str thenEval = _thenBlk->getEval();
        if(!thenEval) return NAMU_E("thenEval is null"), thenEval;
        str elseEval = _elseBlk ? _elseBlk->getEval() : str();
        if(!elseEval) return NAMU_E("elseEval is null"), elseEval;

        if(thenEval->isSub<retStateExpr>())
            return NAMU_DI("thenEval is %s, accept elseEval[%s]",
                           thenEval->getType().getName().c_str(),
                           elseEval ? elseEval->getType().getName().c_str() : "null"), elseEval;
        if(elseEval->isSub<retStateExpr>())
            return NAMU_DI("elseEval is %s, accept thenEval[%s]",
                           elseEval->getType().getName().c_str(),
                           thenEval->getType().getName().c_str()), thenEval;

        // when you try to get eval from ifExpr, `then` and else block must be declared first.
        // if one of blocks has omitted, evaluation of ifExpr should be null.
        str ret = thenEval->deduce(*elseEval); // if elseEval is null, then thenEval only left.
        NAMU_DI("thenEval[%s] + elseVal[%s] -> %s",
                thenEval->getType().getName().c_str(),
                elseEval->getType().getName().c_str(),
                ret ? ret->getType().getName().c_str() : "null");
        return ret;
    }

    void me::setThenBlk(const blockExpr& newThen) {
        _thenBlk.bind(newThen);
    }

    void me::setElseBlk(const blockExpr& newElse) {
        _elseBlk.bind(newElse);
    }

    void me::setCondition(const node& newCondition) {
        _expr.bind(newCondition);
    }
}
