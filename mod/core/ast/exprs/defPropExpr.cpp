#include "defPropExpr.hpp"
#include "../../visitor/visitor.hpp"

namespace nm {
    NAMU(DEF_ME(defPropExpr), DEF_VISIT())

    me::defPropExpr(const std::string& name, const node& rhs): super(name, rhs) {}
    me::defPropExpr(const std::string& name, const node& rhs, const node& to): super(name, rhs, to) {}

    str me::_onMakeNew() const {
        return getRight().as<node>()->run();
    }
}
