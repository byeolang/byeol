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
        str as = getRight() THEN(template as<node>()) orRet str();
        return as->run();
    }
}
