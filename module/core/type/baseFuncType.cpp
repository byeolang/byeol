#include "core/type/baseFuncType.hpp"
#include "core/builtin/primitive/nStr.hpp"
#include "core/type/as/ases.hpp"

namespace by {

    BY(DEF_ME(baseFuncType))

    const std::string& me::getName() const { return TYPE_NAME; }

    namespace {
        struct asStr : public tas<nStr> {
            BY(CLASS(asStr, tas<nStr>))

        public:
            str as(const node& me, const type& to) const override {
                baseFunc& cast = me.cast<baseFunc>() OR.ret(str());
                return new nStr(cast.getSrc().getName() + "(" + cast.getParams().toStr() + ")");
            }
        };
    }

    const ases& me::_getAses() const {
        static ases inner(*new asStr());
        return inner;
    }
}
