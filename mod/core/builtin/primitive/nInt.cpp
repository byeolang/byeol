#include "nInt.hpp"

#include "../../ast/param.hpp"
#include "../../type/as/impliAses.hpp"
#include "../../worker/visitor/visitor.hpp"
#include "bridge/cpp/tbridger.hpp"
#include "nBool.hpp"
#include "nByte.hpp"
#include "nFlt.hpp"

namespace nm {

    NM(DEF_ME(nInt), DEF_VISIT())

    nbool me::wIntType::isImmutable() const { return true; }

    const std::string& me::wIntType::getName() const {
        static std::string inner("int");
        return inner;
    }

    const impliAses& me::wIntType::_getImpliAses() const {
        static impliAses inner{new asPrimitive<nFlt, nint>(), new asPrimitive<nByte, nint>(),
            new asPrimitive<nBool, nint>()};
        return inner;
    }

    namespace {
        // define in unamed namespace in order to avoid symbol duplication.
        struct asStr: public tas<nStr> {
            NM(CLASS(asStr, tas<nStr>))

        public:
            str as(const node& me, const type& to) const override {
                return new nStr(std::to_string(me.cast<nint>()));
            }
        };
    }

    const ases& me::wIntType::_getAses() const {
        static ases inner{new asStr()};
        return inner;
    }

    me::nInt(): super() {}

    me::nInt(nint val): super(val) {}

    tstr<arithmeticObj> me::bitwiseNot() const { return new me(~get()); }

    const baseObj& me::getOrigin() const {
        static tbaseObjOrigin<me> org(tbridger<me>::ctor().ctor<me>().subs());
        const baseObj& supers = super::getOrigin();
        return &supers == this ? org : supers;
    }

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
        return reversed ? new me(rhs.as<me>()->get() & get()) : new me(get() & rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseOr(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() | get()) : new me(get() | rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_bitwiseXor(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() ^ get()) : new me(get() ^ rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_lshift(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() << get()) :
                          new me(get() << rhs.as<me>()->get());
    }

    tstr<arithmeticObj> me::_rshift(const arithmeticObj& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() >> get()) :
                          new me(get() >> rhs.as<me>()->get());
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
