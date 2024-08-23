#include "defBlock.hpp"

#include "../ast/node.hpp"
#include "builtin/container/native/tndumArr.hpp"

namespace nm {

    NM(DEF_ME(defBlock))

    me::defBlock(): _scope(new scope()), _common(new narr()), _postpones(new narr()) {}

    me& me::addCommon(node& stmt) {
        _common->add(&stmt);
        return *this;
    }

    me& me::addScope(const std::string& name, node& stmt) {
        _scope->add(name, stmt);
        return *this;
    }

    me& me::postpone(node& stmt) {
        _postpones->add(stmt);
        return *this;
    }

    const narr& me::getPostpones() const {
        static ndumArr inner;
        return _postpones ? *_postpones : inner;
    }

    scope& me::getScope() { return *_scope; }

    const narr& me::getCommon() const { return *_common; }
}
