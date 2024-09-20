#include "baseCtor.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(baseCtor), DEF_VISIT())

    me::baseCtor(const mgdType& type): _type(type) {}

    str me::getEval() const { return getRet(); }

    const ntype& me::getType() const { return _type; }

    void me::_setOrigin(const node& newOrg) { _type.setRet(newOrg); }
}
