#include "nFlt.hpp"
#include "../../ast/defaultCtor.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "nInt.hpp"
#include "nByte.hpp"
#include "nChar.hpp"
#include "nBool.hpp"
#include "../../visitor/visitor.hpp"

namespace nm {

    NAMU(DEF_ME(nFlt), DEF_VISIT())

    nbool me::wFltType::isImmutable() const { return true; }

    const ases& me::wFltType::_getImpliAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nInt, nflt>());
            inner.add(new asPrimitive<nByte, nflt>());
            inner.add(new asPrimitive<nBool, nflt>());
        }

        return inner;
    }

    const ases& me::wFltType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<nChar, nflt>());
            struct asStr : public tas<nStr> {
                str as(const node& me, const type& to) const override {
                    return str(new nStr(std::to_string(me.cast<nflt>())));
                }
            };
            inner.add(new asStr());
        }

        return inner;
    }

    me::nFlt() {}
    me::nFlt(nflt val): super(val) {}

    void me::_onMakeSubs(scope& tray) const {
        static nFlt inner;
        tray.add(baseObj::CTOR_NAME, new defaultCtor(inner));
        tray.add(baseObj::CTOR_NAME, new defaultCopyCtor(inner));
    }

    tstr<arithmeticObj> me::bitwiseNot() const {
        return *this;
        // TODO: throw error
    }

    const me& me::getOrigin() const {
        static me inner;
        return inner;
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
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_bitwiseAnd(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_bitwiseXor(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_bitwiseOr(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_lshift(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
    }

    tstr<arithmeticObj> me::_rshift(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
        // TODO: throw error
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
