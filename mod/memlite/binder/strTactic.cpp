#include "memlite/binder/strTactic.hpp"

#include "memlite/watcher/bindTag.hpp"
#include "memlite/binder/binder.hpp"

namespace nm {

    NM_DEF_ME(strTactic)

    void me::rel(binder& me) {
        WHEN(!me.isBind()).ret();

        bindTag& tag = me._getBindTag();
        if(!nul(tag)) tag._onStrong(-1);
        super::rel(me);
    }

    nbool me::bind(binder& me, const instance& it) {
        nbool res = super::bind(me, it);
        WHEN(!res).err("super::bind() was failed.").ret(res);
        WHEN(!it.isHeap()).ret(true);

        // initially, when instance got created by new operator, the value 'strong' set to 0.
        // if strongBinder doesn't bind the instance in this circumstance, this instance keep
        // existing in heap area.
        return me._getBindTag()._onStrong(1);
    }

    me me::singleton;
}
