#include "core/ast/exprs/defAssignExpr.hpp"

#include "core/worker/visitor/visitor.hpp"

namespace by {
    NM(DEF_ME(defAssignExpr), DEF_VISIT())

    me::defAssignExpr(const std::string& name, const node* rhs): super(name, rhs) {}

    me::defAssignExpr(const std::string& name, const node* rhs, const node* to, const src& s,
        const modifier& mod):
        me(name, nullptr, rhs, to, s, mod) {}

    me::defAssignExpr(const std::string& name, const node* type, const node* rhs, const node* to,
        const src& s, const modifier& mod):
        super(name, rhs, to, s, mod), _type(type) {}

    str me::getEval() const {
        WHEN(_type).ret(_type->getEval());
        return super::getEval();
    }

    const node* me::getExplicitType() const { return _type.get(); }

    void me::setExplicitType(const node& newType) { _type.bind(newType); }

    str me::_onMakeNew() {
        WHEN(_type).ret(getRight()->as(*_type));

        str ret = getRight() TO(template as<node>());
        BY_I("@%s `%s := %s", this, getName(), *ret);

        return ret;
    }
}
