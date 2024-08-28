#include "nBool.hpp"

#include "../../worker/visitor/visitor.hpp"
#include "bridge/cpp/tbridger.hpp"
#include "nByte.hpp"
#include "nChar.hpp"
#include "nFlt.hpp"
#include "nInt.hpp"
#include "nStr.hpp"

namespace nm {

    NM(DEF_ME(nBool), DEF_VISIT())

    nbool me::wBoolType::isImmutable() const { return true; }

    namespace {
        static tbaseObjOrigin<me> org(/*TODO:*/ dumSrc::singletone(), tbridger<me>::ctor().ctor<me>().subs());
    }

    me::nBool(): super(org) {}

    me::nBool(nbool val): super(val, org) {}

    const ases& me::wBoolType::_getImpliAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nFlt, nbool>());
            inner.add(new asPrimitive<nByte, nbool>());
            inner.add(new asPrimitive<nInt, nbool>());
        }

        return inner;
    }

    const ases& me::wBoolType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nChar, nbool>());

            struct asStr: public tas<nStr> {
                str as(const node& me, const type& to) const override {
                    return str(new nStr(me.cast<nbool>() ? "true" : "false"));
                }
            };

            inner.add(new asStr());
        }

        return inner;
    }

    tstr<arithmeticObj> me::_lshift(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new nInt(rhs.as<me>()->get() << get()) :
                          new nInt(get() << rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_rshift(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new nInt(rhs.as<me>()->get() >> get()) :
                          new nInt(get() >> rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::bitwiseNot() const { return nInt(!get()); }

    tstr<arithmeticObj> me::_add(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() + get()) : new me(get() + rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_sub(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() - get()) : new me(get() - rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_mul(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() * get()) : new me(get() * rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_div(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() / get()) : new me(get() / rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_mod(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() % get()) : new me(get() % rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseAnd(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() && get()) :
                          new me(get() && rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseOr(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() || get()) :
                          new me(get() || rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseXor(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() ^ get()) : new me(get() ^ rhs.as<me>()->get());
    }

    nbool me::_eq(const arithmeticObj& rhs) const { return get() == rhs.asImpli<me>()->get(); }

    nbool me::_ne(const arithmeticObj& rhs) const { return get() != rhs.asImpli<me>()->get(); }

    nbool me::_gt(const arithmeticObj& rhs) const { return get() > rhs.asImpli<me>()->get(); }

    nbool me::_lt(const arithmeticObj& rhs) const { return get() < rhs.asImpli<me>()->get(); }

    nbool me::_ge(const arithmeticObj& rhs) const { return get() >= rhs.asImpli<me>()->get(); }

    nbool me::_le(const arithmeticObj& rhs) const { return get() <= rhs.asImpli<me>()->get(); }

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
} // namespace nm
