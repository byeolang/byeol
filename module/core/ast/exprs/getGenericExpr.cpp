#include "core/ast/exprs/getGenericExpr.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/ast/genericOrigin.hpp"
#include "core/ast/node.inl"

namespace nm {

    NM(DEF_ME(getGenericExpr), DEF_VISIT())

    me::getGenericExpr(const std::string& genericName, const args& typeParams):
        super(genericName, typeParams) {}

    me::getGenericExpr(const node& me, const std::string& genericName, const args& typeParams):
        super(me, genericName, typeParams) {}

    node& me::_onGet(node& me) const {
        const args& typs = getArgs();
        const std::string& name = getName();
        WHEN(nul(typs) || !typs.len()).err("_args.len() == 0").retNul<genericOrigin>();
        NM_DI("_name=%s, _args[%d]", getName(), typs.len());

        node& generic =
            me TO(template sub<genericOrigin>(name)) OR.err("generic == null").retNul<node>();

        return *generic.run(getArgs());
    }
}
