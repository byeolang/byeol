#include "asExpr.hpp"
#include "../../visitor/visitor.hpp"

namespace nm {

    NAMU(DEF_ME(asExpr), DEF_VISIT())

    me::asExpr(const node& me, const node& as): _me(me), _as(as) {}

    str me::run(const args& a) {
        if(!_me || !_as) return str();

        str eval = _me->as<node>();
        if(!eval) return NAMU_E("!eval.isBind()"), str();

        return str(eval->as(*_as));
    }

    str me::getEval() const {
        return _as;
    }

    const node& me::getMe() const {
        return *_me;
    }

    const node& me::getAs() const {
        return *_as;
    }

    void me::setAs(const node& new1) {
        _as.bind(new1);
    }

    clonable* me::cloneDeep() const {
        NAMU_DI("%s.cloneDeep()", getType().getName().c_str());
        me* ret = (me*) clone();
        if(_me) ret->_me.bind((node*) _me->cloneDeep());
        if(_as) ret->_as.bind((node*) _as->cloneDeep());
        return ret;
    }
}
