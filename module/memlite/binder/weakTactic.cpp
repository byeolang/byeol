#include "memlite/binder/weakTactic.hpp"

#include "memlite/binder/binder.hpp"

namespace by {

    BY_DEF_ME(weakTactic)

    void me::rel(binder& me) { memliteInternal::rel(me); }

    nbool me::bind(binder& me, const instance& it) {
        // regardless of result from _onStrong binder can bind:
        //  there are two reasons:
        //      because life has equal lifecycle to what it bind, if there is
        //      a request by user to refer a binder for binding freed instance,
        //      user has responsibilty to treat wrongly. so, we should not consider such cases.
        //
        //  and:
        //      no matter how life reacts, anyway it won't refuse binder's
        //      refering. for instance, the scenario for binding non-heap allocated
        //      instance.
        memliteInternal::setBinderItsId(me, it.getId());
        return true;
    }

    me me::singleton;
}
