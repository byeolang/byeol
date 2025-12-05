#include "core/builtin/scalar/nByte.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/bridge/cpp/tbridger.hpp"
#include "core/builtin/scalar/nBool.hpp"
#include "core/builtin/scalar/nFlt.hpp"
#include "core/builtin/scalar/nInt.hpp"

namespace by {

    BY(DEF_ME(nByte), DEF_VISIT())

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
        struct asStr: public tas<nStr, nByte> {
            typedef tas<nStr, nByte> __super22;
            BY(CLASS(asStr, __super22))

        protected:
            str _onAs(const nByte& me, const type& to) const override {
                return str(new nStr(std::string(1, me.get())));
            }
        };
    }

    const ases& me::nByteType::_getAses() const {
        static ases inner(*new asPrimitive<nFlt, nuchar>(), *new asStr());
        return inner;
    }

    me::nByte(nuchar val): super(val) {}

    tstr<scalar> me::bitwiseNot() const { return new me(~get()); }

    const baseObj& me::getOrigin() const {
        static tbaseObjOrigin<me> org(tbridger<me>::ctor().ctor<me>().subs());
        const baseObj& supers = super::getOrigin();
        return &supers == this ? org : supers;
    }

    tstr<scalar> me::_add(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() + get()) : new me(get() + rhs.as<me>()->get());
    }

    tstr<scalar> me::_sub(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() - get()) : new me(get() - rhs.as<me>()->get());
    }

    tstr<scalar> me::_mul(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() * get()) : new me(get() * rhs.as<me>()->get());
    }

    tstr<scalar> me::_div(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() / get()) : new me(get() / rhs.as<me>()->get());
    }

    tstr<scalar> me::_mod(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() % get()) : new me(get() % rhs.as<me>()->get());
    }

    tstr<scalar> me::_bitwiseAnd(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() & get()) : new me(get() & rhs.as<me>()->get());
    }

    tstr<scalar> me::_bitwiseOr(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() | get()) : new me(get() | rhs.as<me>()->get());
    }

    tstr<scalar> me::_bitwiseXor(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() ^ get()) : new me(get() ^ rhs.as<me>()->get());
    }

    tstr<scalar> me::_lshift(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() << get()) : new me(get() << rhs.as<me>()->get());
    }

    tstr<scalar> me::_rshift(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() >> get()) : new me(get() >> rhs.as<me>()->get());
    }

    nbool me::_eq(const scalar& rhs) const { return get() == rhs.asImpli<me>()->get(); }

    nbool me::_ne(const scalar& rhs) const { return get() != rhs.asImpli<me>()->get(); }

    nbool me::_gt(const scalar& rhs) const { return get() > rhs.asImpli<me>()->get(); }

    nbool me::_lt(const scalar& rhs) const { return get() < rhs.asImpli<me>()->get(); }

    nbool me::_ge(const scalar& rhs) const { return get() >= rhs.asImpli<me>()->get(); }

    nbool me::_le(const scalar& rhs) const { return get() <= rhs.asImpli<me>()->get(); }

    nbool me::_logicalAnd(const scalar& rhs) const { return get() && rhs.asImpli<me>()->get(); }

    nbool me::_logicalOr(const scalar& rhs) const { return get() || rhs.asImpli<me>()->get(); }

    scalar& me::_mov(const scalar& rhs) {
        get() = rhs.asImpli<me>()->get();
        return *this;
    }
} // namespace by
