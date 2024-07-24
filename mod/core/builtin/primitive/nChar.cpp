#include "nChar.hpp"
#include "bridge/cpp/tbridger.hpp"
#include "nInt.hpp"
#include "nFlt.hpp"
#include "nBool.hpp"
#include "nByte.hpp"
#include "../../visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(nChar), DEF_VISIT())

    nbool me::nCharType::isImmutable() const { return true; }

    const ases& me::nCharType::_getImpliAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nBool, nchar>());
            inner.add(new asPrimitive<nInt, nchar>());
            inner.add(new asPrimitive<nByte, nchar>());
            struct asStr : public tas<nStr> {
                str as(const node& me, const type& to) const override {
                    std::string val;
                    val.push_back(me.cast<nchar>());
                    return str(new nStr(val));
                }
            };
            inner.add(new asStr());
        }

        return inner;
    }

    const ases& me::nCharType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nFlt, nchar>());
        }

        return inner;
    }

    me::nChar() {}
    me::nChar(nchar val): super(val) {}

    namespace {
        static me org;
    }

    scope& me::_onMakeSubs() const {
        static scope inner = tbridger<me>::ctor().ctor<me>().subs();
        return inner;
    }

    tstr<arithmeticObj> me::bitwiseNot() const {
        return new me(~get());
    }

    const baseObj& me::getOrigin() const {
        return org;
    }

    tstr<arithmeticObj> me::_add(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() + get()):
                new me(get() + rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_sub(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() - get()):
                new me(get() - rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_mul(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() * get()):
                new me(get() * rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_div(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() / get()):
                new me(get() / rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_mod(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() % get()):
                new me(get() % rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseAnd(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() & get()):
                new me(get() & rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseOr(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() | get()):
                new me(get() | rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseXor(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() ^ get()):
                new me(get() ^ rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_lshift(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() << get()):
                new me(get() << rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_rshift(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ?
                new me(rhs.as<me>()->get() >> get()):
                new me(get() >> rhs.as<me>()->get());
    }

    nbool me::_eq(const arithmeticObj& rhs) const {
        return get() == rhs.asImpli<me>()->get();
    }

    nbool me::_ne(const arithmeticObj& rhs) const {
        return get() != rhs.asImpli<me>()->get();
    }

    nbool me::_gt(const arithmeticObj& rhs) const {
        return get() > rhs.asImpli<me>()->get();
    }

    nbool me::_lt(const arithmeticObj& rhs) const {
        return get() < rhs.asImpli<me>()->get();
    }

    nbool me::_ge(const arithmeticObj& rhs) const {
        return get() >= rhs.asImpli<me>()->get();
    }

    nbool me::_le(const arithmeticObj& rhs) const {
        return get() <= rhs.asImpli<me>()->get();
    }

    nbool me::_logicalAnd(const arithmeticObj& rhs) const {
        return get() && rhs.asImpli<me>()->get();
    }

    nbool me::_logicalOr(const arithmeticObj& rhs) const {
        return get() || rhs.asImpli<me>()->get();
    }

    arithmeticObj& me::_mov(const arithmeticObj& rhs) {
        get() = rhs.asImpli<me>()->get();
        return *this;
    }
}
