#include "baseFuncAses.hpp"
#include "core/frame/thread.hpp"
#include "core/ast/ctor/ctor.hpp"
#include "core/builtin/primitive/nStr.hpp"

namespace by {
    str baseFuncAsStr::_onAs(const baseFunc& me, const type& to) const {
        return new nStr(me.getSrc().getName() + "(" + me.getParams().toStr() + ")");
    }

    nbool baseFuncAsBaseFunc::is(const type& from, const type& to) const {
        WHEN(from.getName() == ttype<ctor>::get().getName()) .ret(false); // ctor can't be casted to any func.
        // TODO: not support closure by `as` yet, but it's TODO.
        // TODO: WHEN(from.getName() == ttype<closure>::get().getName()).ret(false);

        return from == to;
    }

    str baseFuncAsBaseFunc::_onAs(const baseFunc& me, const type& to) const {
        // if you're about to run this func, it means that our libmeta confirm that this
        // is castable action. so don't try to call `is()` again.
        return me;
    }

    /*str baseFuncAsClosure::_onAs(const baseFunc& me, const type& to) const {
        WHEN(me.isSub<closure>()).ret(me);
        baseObj& meObj = thread::get().getNowFrame() TO(getMe()) TO(template cast<baseObj>()) OR.ret(str());
        return new closure(meObj, me);
    }*/
}
