#include "baseObjOrigin.hpp"
#include "obj.hpp"

namespace nm {
    NM(DEF_ME(baseObjOrigin))

    me::baseObjOrigin(const src& s, const scope& subs): super(), _src(s), _subs(subs) {}

    const obj& me::getSubPack() const {
        static obj inner;
        return inner;
    }

    const src& me::getSrc() const { return *_src; }

    const baseObj& me::getOrigin() const { return *this; }

    scope& me::subs() { return *_subs; }
}
