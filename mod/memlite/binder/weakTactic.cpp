#include "weakTactic.hpp"
#include "binder.hpp"

namespace nm {

    NM_DEF_ME(weakTactic)

    void me::rel(binder& me) {
        me._itsId.rel();
    }

    nbool me::bind(binder& me, const instance& it) {
        // regardless of result from _onStrong binder can bind:
        //  there are two reasons:
        //      because bindTag has equal lifecycle to what it bind, if there is
        //      a request by user to refer a binder for binding freed instance,
        //      user has responsibilty to treat wrongly. so, we should not consider such cases.
        //
        //  and:
        //      no matter how bindTag reacts, anyway it won't refuse binder's
        //      refering. for instance, the scenario for binding non-heap allocated
        //      instance.
        me._itsId = it.getId();
        return true;
    }

    me me::singletone;
}
