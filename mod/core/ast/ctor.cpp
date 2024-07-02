#include "ctor.hpp"
#include "../visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(ctor), DEF_VISIT())

    me::ctor(const node& org): _org(org) {}

    str me::getRet() const {
        return _org->getEval();
    }

    str me::getEval() const {
        return getRet();
    }

    const node& me::getOrigin() const {
        return *_org;
    }

    void me::_setOrigin(const node& newOrg) {
        _org.bind(newOrg);
    }
}
