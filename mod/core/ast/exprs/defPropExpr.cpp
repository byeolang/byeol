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
        str ased = getRight().as<node>();
        closure& cast = ased->cast<closure>();
        if(!nul(cast)) return cast;
        if(ased->isSub<baseFunc>()) return closure::make(*ased);

        return ased->run();
    }
}
