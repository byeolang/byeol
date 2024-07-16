#include "nInt.hpp"
#include "../../ast/param.hpp"
#include "../../ast/defaultCtor.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "nFlt.hpp"
#include "nByte.hpp"
#include "nChar.hpp"
#include "nBool.hpp"
#include "../../visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(nInt), DEF_VISIT())

    void me::_onMakeSubs(scope& tray) const {
        static nInt inner;
        tray.add(baseObj::CTOR_NAME, new defaultCtor(inner));
        tray.add(baseObj::CTOR_NAME, new defaultCopyCtor(inner));
    }

    nbool me::wIntType::isImmutable() const { return true; }

    const ases& me::wIntType::_getImpliAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nFlt, nint>());
            inner.add(new asPrimitive<nByte, nint>());
            inner.add(new asPrimitive<nBool, nint>());
        }

        return inner;
    }

    const ases& me::wIntType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nChar, nint>());
            struct asStr : public tas<nStr> {
                str as(const node& me, const type& to) const override {
                    return new nStr(std::to_string(me.cast<nint>()));
                }
            };
            inner.add(new asStr());
        }

        return inner;
    }

    me::nInt() {}
    me::nInt(nint val): super(val) {}

    tstr<arithmeticObj> me::bitwiseNot() const {
        return new me(~get());
    }

    const obj& me::getOrigin() const {
        // TODO:
        return nulOf<obj>();
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
