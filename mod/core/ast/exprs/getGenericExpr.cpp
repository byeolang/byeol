#include "getGenericExpr.hpp"

#include "../../worker/visitor/visitor.hpp"
#include "../genericOrigin.hpp"
#include "../node.inl"

namespace nm {

    NM(DEF_ME(getGenericExpr), DEF_VISIT())

    me::getGenericExpr(const std::string& genericName, const args& typeParams):
        super(genericName, typeParams) {}

    me::getGenericExpr(const node& me, const std::string& genericName, const args& typeParams):
        super(me, genericName, typeParams) {}

    priorities me::_get(nbool evalMode) const {
        genericOrigin& generic = _getGenericOrigin();
        if(nul(generic)) return NM_E("generic == null"), priorities();

        return priorities(*generic.run(getArgs()));
    }

    genericOrigin& me::_getGenericOrigin() const {
        const args& typs = getArgs();
        const std::string& name = getName();
        if(nul(typs) || !typs.len()) return NM_E("_args.len() == 0"), nulOf<genericOrigin>();
        NM_DI("_name=%s, _args[%d]", getName(), typs.len());

        str evalMe = getMe().isSub<expr>() ? getMe().as<node>() : getMe();
        if(!evalMe) return NM_E("from == null"), nulOf<genericOrigin>();

        return evalMe->sub<genericOrigin>(name);
    }
}
