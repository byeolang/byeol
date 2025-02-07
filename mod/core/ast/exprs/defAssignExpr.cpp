#include "defAssignExpr.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(defAssignExpr), DEF_VISIT())

    me::defAssignExpr(const std::string& name, const node& rhs): super(name, rhs) {}

    me::defAssignExpr(const std::string& name, const node& rhs, const node& to, const src& s,
        const modifier& mod):
        me(name, nulOf<node>(), rhs, to, s, mod) {}

    me::defAssignExpr(const std::string& name, const node& type, const node& rhs, const node& to,
        const src& s, const modifier& mod):
        super(name, rhs, to, s, mod), _type(type) {}

    str me::getEval() const {
        if(_type) return _type->getEval();
        return super::getEval();
    }

    str me::getExplicitType() const { return *_type; }

    void me::setExplicitType(const node& newType) { _type.bind(newType); }

    str me::_onMakeNew() {
        if(_type) return getRight().as(*_type);
        str ret = getRight().as<node>();
        NM_I("@%s `%s := %s", this, getName(), *ret);
        return ret;
    }
}
