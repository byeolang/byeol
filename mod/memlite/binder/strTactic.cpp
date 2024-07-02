#include "strTactic.hpp"
#include "binder.hpp"
#include "../watcher/bindTag.hpp"

namespace nm {

    NM_DEF_ME(strTactic)

    void me::rel(binder& me) {
        if(!me.isBind()) return;

        bindTag& tag = me._getBindTag();
        if(!nul(tag))
            tag._onStrong(-1);
        super::rel(me);
    }

    nbool me::bind(binder& me, const instance& it) {
        nbool res = super::bind(me, it);
        if(!res) {
            NM_E("super::bind() was failed.");
            return res;
        }

        if(!it.isHeap())
            return true;

        // initially, when instance got created by new operator, the value 'strong' set to 0.
        // if strongBinder doesn't bind the instance in this circumstance, this instance keep
        // existing in heap area.
        return me._getBindTag()._onStrong(1);
    }

    me me::singletone;
}
