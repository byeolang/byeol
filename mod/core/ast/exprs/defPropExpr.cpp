#include "defPropExpr.hpp"

namespace namu {
    NAMU(DEF_ME(defPropExpr))

    me::defPropExpr(const std::string& name, const node& rhs): super(name, rhs) {}
    me::defPropExpr(const std::string& name, const node& rhs, const node& to): super(name, rhs, to) {}

    str me::_onMakeNew() const {
        return getRight().as<node>()->run();
    }
}
