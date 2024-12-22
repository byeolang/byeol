#include "defPropExpr.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(defPropExpr), DEF_VISIT())

    me::defPropExpr(const std::string& name, const node& rhs): super(name, rhs) {}

    me::defPropExpr(const std::string& name, const node& rhs, const node& to, const src& s,
        const modifier& mod):
        super(name, rhs, to, s, mod) {}

    str me::_onMakeNew() {
        str ased = getRight().as<node>();
        if(ased->isSub<baseFunc>()) return ased;

        return ased->run();
    }
}
