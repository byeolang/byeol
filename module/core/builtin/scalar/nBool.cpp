#include "core/builtin/scalar/nBool.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/bridge/cpp/tbridger.hpp"
#include "core/builtin/scalar/nByte.hpp"
#include "core/builtin/scalar/nFlt.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"

namespace by {

    BY(DEF_ME(nBool), DEF_VISIT())

    nbool me::wBoolType::isImmutable() const { return true; }

    const std::string& me::wBoolType::getName() const {
        static std::string inner("bool");
        return inner;
    }

    me::nBool(nbool val): super(val) {}

    const impliAses& me::wBoolType::_getImpliAses() const {
        static impliAses inner{new asPrimitive<nFlt, nbool>(), new asPrimitive<nByte, nbool>(),
            new asPrimitive<nInt, nbool>()};
        return inner;
    }

    namespace {
        struct asStr: public tas<nStr, me> {
            typedef tas<nStr, me> __super10;
            BY(CLASS(asStr, __super10))

        protected:
            str _onAs(const nBool& me, const type& to) const override {
                return str(new nStr(me.get() ? "true" : "false"));
            }
        };
    }

    const ases& me::wBoolType::_getAses() const {
        static ases inner(*new asStr());
        return inner;
    }

    tstr<scalar> me::_lshift(const scalar& rhs, nbool reversed) const {
        return reversed ? new nBool((rhs.as<me>()->get() << get()) != 0) :
                          new nBool((get() << rhs.as<me>()->get()) != 0);
    }

    tstr<scalar> me::_rshift(const scalar& rhs, nbool reversed) const {
        return reversed ? new nBool((rhs.as<me>()->get() >> get()) != 0) :
                          new nBool((get() >> rhs.as<me>()->get()) != 0);
    }

    tstr<scalar> me::bitwiseNot() const { return nInt(!get()); }

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
        return reversed ? new me(rhs.as<me>()->get() && get()) : new me(get() && rhs.as<me>()->get());
    }

    tstr<scalar> me::_div(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() && get()) : new me(get() && rhs.as<me>()->get());
    }

    tstr<scalar> me::_mod(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() % get()) : new me(get() % rhs.as<me>()->get());
    }

    tstr<scalar> me::_bitwiseAnd(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() && get()) : new me(get() && rhs.as<me>()->get());
    }

    tstr<scalar> me::_bitwiseOr(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() || get()) : new me(get() || rhs.as<me>()->get());
    }

    tstr<scalar> me::_bitwiseXor(const scalar& rhs, nbool reversed) const {
        return reversed ? new me(rhs.as<me>()->get() ^ get()) : new me(get() ^ rhs.as<me>()->get());
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
