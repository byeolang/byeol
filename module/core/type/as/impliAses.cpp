#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/container/native/tnmap.inl"
#include "core/type/as/impliAses.hpp"
#include "core/ast/node.hpp"
#include "core/builtin/container/native/tnarr.inl"

namespace by {

    BY(DEF_ME(impliAses))

    namespace {
        class derivedAser: public aser {
            BY(CLASS(derivedAser, aser))

        public:
            nbool is(const type& from, const type& to) const override { return to.isSuper(from); }

            str as(const node& me, const type& to) const override { return me; }
        };
    }

    me::impliAses() { add(new derivedAser()); }

    me::impliAses(const std::initializer_list<aser*>& args): me() {
        for(aser* arg: args)
            add(arg);
    }
}
