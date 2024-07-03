#include "getGenericExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../node.inl"
#include "../genericObj.hpp"

namespace nm {

    NM(DEF_ME(getGenericExpr), DEF_VISIT())

    me::getGenericExpr(const std::string& genericName, const args& typeParams):
            super(genericName, typeParams) {}
    me::getGenericExpr(const node& me, const std::string& genericName, const args& typeParams):
            super(me, genericName, typeParams) {}

    priorities me::_get(nbool evalMode) const {
        genericObj& generic = _getGenericObj();
        if(nul(generic)) return NM_E("generic == null"), priorities();

        return priorities(*generic.run(getArgs()));
    }

    genericObj& me::_getGenericObj() const {
        const args& typs = getArgs();
        const std::string& name = getName();
        if(nul(typs) || !typs.len()) return NM_E("_args.len() == 0"), nulOf<genericObj>();
        NM_DI("_name=%s, _args[%s]", getName(), typs.len());

        str evalMe = getMe().isSub<expr>() ? getMe().as<node>() : getMe();
        if(!evalMe) return NM_E("from == null"), nulOf<genericObj>();

        return evalMe->sub<genericObj>(name);
    }
}
