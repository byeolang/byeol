#include "core/worker/defBlock.hpp"
#include "core/ast/node.hpp"
#include "core/builtin/container/native/tndumArr.hpp"

namespace nm {

    NM(DEF_ME(defBlock))

    me::defBlock(): _scope(new scope()), _common(new narr()), _expands(new narr()) {}

    me& me::addCommon(node& stmt) {
        _common->add(&stmt);
        return *this;
    }

    me& me::addScope(const std::string& name, node& stmt) {
        _scope->add(name, stmt);
        return *this;
    }

    me& me::expand(node& stmt) {
        _expands->add(stmt);
        return *this;
    }

    const narr& me::getExpands() const {
        static ndumArr inner;
        return _expands ? *_expands : inner;
    }

    scope& me::getScope() { return *_scope; }

    const narr& me::getCommon() const { return *_common; }
}
