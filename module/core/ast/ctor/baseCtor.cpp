#include "core/ast/ctor/baseCtor.hpp"
#include "core/ast/tmock.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(baseCtor), DEF_VISIT())

    me::baseCtor(const mgdType& type): _type(type) {}

    str me::infer() const { return getRet(); }

    const ntype& me::getType() const { return _type; }

    const baseObj& me::getOrigin() const {
        static tmock<baseObj> dummy;
        return _type.getRet() TO(template cast<baseObj>()) OR.ret(dummy);
    }
}
