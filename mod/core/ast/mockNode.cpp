#include "mockNode.hpp"
#include "../builtin/container/native/tndumMap.hpp"
#include "../visitor/visitor.hpp"
#include "dumScope.hpp"

namespace namu {

    NAMU(DEF_ME(mockNode), DEF_VISIT())

    me::mockNode(): super() {}
    me::mockNode(const node& org): super(), _org(org) {}

    const ntype& me::getType() const {
        if(_org) return _org->getType();
        return ttype<node>::get();
    }

    priorType me::prioritize(const args& a) const {
        if(_org) return _org->prioritize(a);
        return NO_MATCH;
    }

    str me::run(const args& a) {
        if(_org) return _org->run(a);
        return str();
    }

    clonable* me::clone() const {
        if(_org) return _org->clone();
        return new me(*this);
    }

    scope& me::subs() {
        if(_org) return _org->subs();
        static dumScope inner;
        return inner;
    }

    nbool me::isComplete() const {
        return true; // always. this's the main reason why I use this class.
    }

    node& me::getTarget() {
        return *_org;
    }
}
