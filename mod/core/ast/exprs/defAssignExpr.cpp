#include "defAssignExpr.hpp"

namespace namu {
    NAMU(DEF_ME(defAssignExpr))

    me::defAssignExpr(const std::string& name, const node& rhs): super(name, rhs) {}
    me::defAssignExpr(const std::string& name, const node& rhs, const node& to): super(name, rhs, to) {}

    str me::_onMakeNew() const {
        return getRight().as<node>();
    }
}
