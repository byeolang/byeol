#include "weakTactic.hpp"
#include "binder.hpp"

namespace wrd {

    WRD_DEF_ME(weakTactic)

    wbool me::rel(binder& me) {
        me._itsId.rel();
        return true;
    }

    wbool me::bind(binder& me, const instance& it) {
        //  regardless of result from _onStrong binder can bind:
        //      there are two reasons:
        //          because Block has equal lifecycle to what it bind, if there is
        //          a request by user to refer a binder for binding freed instance,
        //          user has responsibilty to treat wrongly.
        //          so, we should not consider such cases.
        //
        //      and:
        //          no matter how block reacts, anyway it won't refuse binder's
        //          refering. for instance, the scenario for binding non-heap allocated
        //          instance.
        me._itsId = it.getId();
        return true;
    }

    me me::singletone;
}
