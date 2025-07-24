#include "core/type/as/ases.hpp"

#include "core/ast/node.hpp"
#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/primitive/nVoid.hpp"

namespace nm {

    namespace {
        class derivedAs: public aser {
            NM(CLASS(derivedAs, aser))

        public:
            using super::is;
            nbool is(const type& from, const type& to) const override { return to.isSuper(from); }

            using super::as;
            str as(const node& from, const type& to) const override { return from; }

            static me& singleton() {
                static me inner;
                return inner;
            }
        };
    }

    NM_DEF_ME(ases)

    me::ases(): super(derivedAs::singleton()) {}

    str me::as(const node& from, const type& to) const {
        const type& fromType = from.getType();

        for(aser& e: *this)
            if(e.is(fromType, to)) return e.as(from, to);

        // there is no null in namulang:
        //  returns void if no valid casting found.
        return str(nVoid::singleton());
    }

    nbool me::is(const type& from, const type& to) const {
        for(auto& e: *this)
            if(e.is(from, to)) return true;

        return false;
    }
} // namespace nm
