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

    node& me::_onGet(baseObj& me) const {
        const args& typs = getArgs();
        const std::string& name = getName();
        if(nul(typs) || !typs.len()) return NM_E("_args.len() == 0"), nulOf<genericOrigin>();
        NM_DI("_name=%s, _args[%d]", getName(), typs.len());

        node& generic = me.sub<genericOrigin>(name);
        if(nul(generic)) return NM_E("generic == null"), nulOf<node>();

        return *generic.run(getArgs());
    }
}
