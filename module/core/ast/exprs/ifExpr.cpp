#include "core/ast/exprs/ifExpr.hpp"

#include "core/builtin/primitive/nBool.hpp"
#include "core/builtin/primitive/nVoid.hpp"
#include "core/frame/frameInteract.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/exprs/retStateExpr.hpp"

namespace by {

    BY(DEF_ME(ifExpr), DEF_VISIT())

    me::ifExpr(const node& exp, const blockExpr& thenBlk): _expr(exp), _then(thenBlk) {}

    me::ifExpr(const node& exp, const blockExpr& thenBlk, const blockExpr& elseBlk):
        _expr(exp), _then(thenBlk), _else(elseBlk) {}

    blockExpr& me::getThen() { return *_then; }

    blockExpr* me::getElse() { return _else.get(); }

    node& me::getCondition() { return *_expr; }

    str me::run(const args& a) {
        tstr<nBool> res = _expr->as<node>() TO(template asImpli<nBool>()) OR.ret(nVoid::singleton());
        nbool cond = *res->cast<nbool>();
        BY_DI("@%s `if %s --> to %s`", this, *_expr, cond ? "THEN" : "ELSE");
        auto* blk = cond ? _then.get() : _else.get();
        if(blk) {
            frameInteract f1(blk);
            return blk->run();
        }

        return str(nVoid::singleton());
    }

    str me::infer() const {
        str thenEval = _then->infer() OR.err("thenEval is null").ret(thenEval);
        str elseEval = (_else ? _else->infer() : str()) OR.err("elseEval is null").ret(elseEval);

        WHEN(thenEval->isSub<retStateExpr>())
            .info("thenEval is %s, accept elseEval[%s]", thenEval, elseEval).ret(elseEval);
        WHEN(elseEval->isSub<retStateExpr>())
            .info("elseEval is %s, accept thenEval[%s]", elseEval, thenEval).ret(thenEval);

        // when you try to get eval from ifExpr, `then` and else block must be declared first.
        // if one of blocks has omitted, evaluation of ifExpr should be null.
        str ret = thenEval->promote(*elseEval); // if elseEval is null, then thenEval only left.
        BY_DI("thenEval[%s] + elseVal[%s] -> %s", thenEval, elseEval, ret);
        return ret;
    }

    void me::setThen(const blockExpr& newThen) { _then.bind(newThen); }

    void me::setElse(const blockExpr& newElse) { _else.bind(newElse); }

    void me::setCondition(const node& newCondition) { _expr.bind(newCondition); }
} // namespace by
