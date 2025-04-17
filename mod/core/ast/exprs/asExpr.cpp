#include "asExpr.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(asExpr), DEF_VISIT())

    me::asExpr(const node& me, const node& as): _me(me), _as(as) {}

    str me::run(const args& a) {
        WHEN(!_me || !_as).ret(str());

        str eval = _me->as<node>() OR_RET NM_E("!eval.isBind()"), str();

        return str(eval->as(*_as->getEval()));
    }

    str me::getEval() const { return _as; }

    const node& me::getMe() const { return *_me; }

    const node& me::getAs() const { return *_as; }

    void me::setAs(const node& new1) { _as.bind(new1); }

    void me::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        if(rhs._me) _me.bind((node*) rhs._me->cloneDeep());
        if(rhs._as) _as.bind((node*) rhs._as->cloneDeep());
    }
} // namespace nm
