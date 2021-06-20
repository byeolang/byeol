#include "strTactic.hpp"
#include "binder.hpp"
#include "../watcher/bindTag.hpp"

namespace wrd {

    WRD_DEF_ME(strTactic)

    wbool me::unbind(binder& me) {
        if(!me.isBind()) return true;

        bindTag& tag = me._getBindTag();
        WRD_NUL(tag, super::unbind(me));

        tag._onStrong(-1);
        return super::unbind(me);
    }

    wbool me::bind(binder& me, const instance& it) {
        wbool res = super::bind(me, it);
        if(!res) {
            WRD_E("super::bind() was failed.");
            return res;
        }

        if(!it.isHeap())
            return true;

        //  처음에 instance가 instancer에 생성되었을때는 strong==0 이며,
        //  StrongBinder가 붙지 않는다면 그대로 계속 메모리상주하게 된다.
        //  Strong이 Count.strong=0인 instance를 bind하는 순간, 이 instance는
        //  binder에 의해서 해제될 수 있게 된다.
        return me._getBindTag()._onStrong(1);
    }

    me me::singletone;
}
