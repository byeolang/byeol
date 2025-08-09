#include "core/ast/ctor/baseCtor.hpp"

#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(baseCtor), DEF_VISIT())

    me::baseCtor(const mgdType& type): _type(type) {}

    str me::getEval() const { return getRet(); }

    const ntype& me::getType() const { return _type; }

    const baseObj& me::getOrigin() const {
        static tmock<baseObj> dummy;
        return _type.getRet() TO(template cast<baseObj>()) OR.ret(dummy);
    }

    void me::_setOrigin(const baseObj& newOrg) { _type.setRet(newOrg); }
}
