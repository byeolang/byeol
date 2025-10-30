#include "core/type/baseFuncType.hpp"
#include "core/builtin/primitive/nStr.hpp"
#include "core/type/as/ases.hpp"

namespace by {

    BY(DEF_ME(baseFuncType))

    namespace {
        struct asStr : public tas<nStr, baseFunc> {
            typedef tas<nStr, baseFunc> __super12;
            BY(CLASS(asStr, __super12))

        protected:
            str _onAs(const baseFunc& me, const type& to) const override {
                return new nStr(me.getSrc().getName() + "(" + me.getParams().toStr() + ")");
            }
        };
    }

    const ases& me::_getAses() const {
        static ases inner(*new asStr());
        return inner;
    }
}
