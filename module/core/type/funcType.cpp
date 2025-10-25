#include "core/type/funcType.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/builtin/primitive/nStr.hpp"

namespace by {
    BY(DEF_ME(funcType))

    me::funcType(const std::string& name, const type& superType, const params& ps, nbool isAdt, const node* ret):
        super(name, superType, ps, isAdt, ret) {}

    struct asFunc: public aser {
        BY(CLASS(asFunc, aser))

    public:
        nbool is(const type& from, const type& to) const override {
            WHEN(to.getMetaTypeName() != ntype::META_TYPENAME) .ret(false);
            WHEN(to.getName() != ttype<baseFunc>::get().getName()).ret(false);

            // okay. it's func:
            //  param, returnType should be exactly matched to.
            ntype& castTo = (ntype&) to;
            ntype& castFrom = (ntype&) from;
            //      param:
            const params& lhsPs = castFrom.getParams();
            const params& rhsPs = castTo.getParams();
            WHEN(lhsPs.len() != rhsPs.len()) .ret(false);
            for(nidx n = 0; n < lhsPs.len(); n++)
                WHEN(lhsPs[n] != rhsPs[n]) .ret(false);
            //      retType:
            const node& lhsRet = castFrom.getRet() OR.ret(false);
            return lhsRet.getType() == castTo.getRet()->getType();
        }

        str as(const node& me, const type& to) const override {
            // if you're about to run this func, it means that our libmeta confirm that this
            // is castable action. so don't try to call `is()` again.
            return me;
        }
    };

    const impliAses& me::_getImpliAses() const {
        static impliAses inner{new asFunc()};
        return inner;
    }

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
} // namespace by
