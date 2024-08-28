#include "defAssignExpr.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(defAssignExpr), DEF_VISIT())

    me::defAssignExpr(const std::string& name, const node& rhs): super(name, rhs) {}

    me::defAssignExpr(const std::string& name, const node& rhs, const node& to, const src& s,
        const modifier& mod):
        super(name, rhs, to, s, mod) {}

    tstr<baseObj> me::_onMakeNew() const { return getRight().as<baseObj>(); }
}
