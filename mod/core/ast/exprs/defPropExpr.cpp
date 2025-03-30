#include "defPropExpr.hpp"

#include "../../worker/visitor/visitor.hpp"
#include "../closure.hpp"

namespace nm {
    NM(DEF_ME(defPropExpr), DEF_VISIT())

    me::defPropExpr(const std::string& name, const node& rhs): super(name, rhs) {}

    me::defPropExpr(const std::string& name, const node& rhs, const node& to, const src& s,
        const modifier& mod):
        super(name, rhs, to, s, mod) {}

    str me::_onMakeNew() {
        str as = getRight() TO(template as<node>()) orRet str();
        as = as->isSub<baseFunc>() ? as : as->run();
        NM_DI("@%s `%s %s`", this, getName(), *as);
        return as;
    }
}
