#include "isExpr.hpp"
#include "../../builtin/primitive/nBool.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(isExpr), DEF_VISIT())

    me::isExpr(const node& me, const node& to): _me(me), _to(to) {}

    str me::run(const args& a) {
        if(!_me || !_to) return str();

        str eval = _me->as<node>();
        if(!eval) return NM_E("!eval.isBind()"), str();

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
