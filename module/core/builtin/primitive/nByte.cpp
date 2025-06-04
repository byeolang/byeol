#include "core/builtin/primitive/nByte.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/bridge/cpp/tbridger.hpp"
#include "core/builtin/primitive/nBool.hpp"
#include "core/builtin/primitive/nFlt.hpp"
#include "core/builtin/primitive/nInt.hpp"

namespace nm {

    NM(DEF_ME(nByte), DEF_VISIT())

    nbool me::nByteType::isImmutable() const { return true; }

    const std::string& me::nByteType::getName() const {
        static std::string inner("byte");
        return inner;
    }

    const impliAses& me::nByteType::_getImpliAses() const {
        static impliAses inner{new asPrimitive<nBool, nuchar>(), new asPrimitive<nInt, nuchar>()};
        return inner;
    }

    namespace {
        // define in unamed namespace in order to avoid symbol duplication.
        struct asStr: public tas<nStr> {
            NM(CLASS(asStr, tas<nStr>))

        public:
            str as(const node& me, const type& to) const override {
                return str(new nStr(std::string(1, me.cast<nuchar>())));
            }
        };
    }

    const ases& me::nByteType::_getAses() const {
        static ases inner{new asPrimitive<nFlt, nuchar>(), new asStr()};
        return inner;
    }

    me::nByte(nuchar val): super(val) {}

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
