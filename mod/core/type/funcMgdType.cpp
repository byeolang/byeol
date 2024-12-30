#include "funcMgdType.hpp"
#include "as/ases.hpp"

namespace nm {
    NM(DEF_ME(funcMgdType))

    me::funcMgdType(const std::string& name, const type& superType, const params& ps, nbool isAdt,
        const node& ret):
        super(name, superType, ps, isAdt, ret) {}

    struct asFunc: public aser {
        NM(CLASS(asFunc, aser))

    public:
        nbool is(const type& from, const type& to) const override {
            if(to.getMetaTypeName() != ntype::META_TYPENAME) return false;

            // okay. it's func:
            //  param, returnType should be exactly matched to.
            ntype& castTo = (ntype&) to;
            ntype& castFrom = (ntype&) from;
            //      param:
            const params& lhsPs = castFrom.getParams();
            const params& rhsPs = castTo.getParams();
            if(lhsPs.len() != rhsPs.len()) return false;
            for(nidx n = 0; n < lhsPs.len(); n++)
                if(lhsPs[n] != rhsPs[n]) return false;
            //      retType:
            const node& lhsRet = castFrom.getRet() orRet false;
            return lhsRet.getType() == castTo.getRet().getType();
        }

        str as(const node& me, const type& to) const override {
            // if you're about to run this func, it means that our libmeta confirm that this
            // is castable action. so don't try to call `is()` again.
            return me;
        }
    };

    const ases& me::_getImpliAses() const {
        static ases inner;
        if(inner.len() <= 0) inner.add(new asFunc());

        return inner;
    }
} // namespace nm
