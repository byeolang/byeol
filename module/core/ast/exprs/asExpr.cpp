#include "core/ast/exprs/asExpr.hpp"

#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(asExpr), DEF_VISIT())

    me::asExpr(const node& me, const node& as): _me(me), _as(as) {}

    str me::eval(const args& a) {
        // TODO: remove WHEN(!_me || !_as).ret(str());

        str eval = _me->as<node>() OR.err("!eval.isBind()").ret(str());

        return str(eval->as(*_as->infer()));
    }

    str me::infer() const { return _as; }

    const node& me::getMe() const { return *_me; }

    const node& me::getAs() const { return *_as; }

    void me::setAs(const node& new1) { _as.bind(new1); }

    void me::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        /*TODO: remove if(rhs._me)*/ _me.bind((node*) rhs._me->cloneDeep());
        /*TODO: remove if(rhs._as)*/ _as.bind((node*) rhs._as->cloneDeep());
    }
} // namespace by
