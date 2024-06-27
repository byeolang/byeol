#include "defPropExpr.hpp"
#include "../../frame/thread.hpp"

namespace namu {
    NAMU(DEF_ME(defPropExpr))

    me::defPropExpr(const std::string& name, const node& rhs): _name(name), _rhs(rhs) {}
    me::defPropExpr(const std::string& name, const node& rhs, const node& to): _name(name), _rhs(rhs), _to(to) {}

    str me::run(const args& a) {
        str new1 = getRight().as<node>();
        if(!new1)
            return NAMU_E("new1 is null"), str();

        _getToScope().add(_name, *new1);
        return new1;
    }

    const std::string& me::getName() const { return _name; }

    node& me::getRight() { return *_rhs; }
    void me::setRight(const node& rhs) { _rhs.bind(rhs); }

    node& me::getTo() { return *_to; }
    void me::setTo(const node& to) { _to.bind(to); }

    nbool me::isToFrame() const { return !_to; }

    str me::getEval() const { return _rhs->getEval(); }

    clonable* me::cloneDeep() const {
        NAMU_DI("%s.cloneDeep()", getType().getName().c_str());

        me* ret = (me*) clone();
        if(_to) ret->_to.bind((node*) _to->cloneDeep());
        if(_rhs) ret->_rhs.bind((node*) _rhs->cloneDeep());

        return ret;
    }

    scope& me::_getToScope() {
        node& to = getTo();
        return nul(to) ? thread::get()._getNowFrame().getLocals() : to.subs();
    }
}
