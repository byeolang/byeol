#include "core/ast/exprs/isExpr.hpp"
#include "core/builtin/primitive/nBool.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {
    BY(DEF_ME(isExpr), DEF_VISIT())

    me::isExpr(const node& me, const node& to): _me(me), _to(to) {}

    str me::run(const args& a) {
        WHEN(!_me || !_to) .ret(str());

        str eval = _me->as<node>() OR.err("!eval.isBind()").ret(str());

        return new nBool(eval->is(*_to->getEval()));
    }

    str me::getEval() const {
        static nBool inner;
        return inner;
    }

    const node& me::getMe() const { return *_me; }

    const node& me::getTo() const { return *_to; }

    void me::setTo(const node& new1) { _to.bind(new1); }
}
