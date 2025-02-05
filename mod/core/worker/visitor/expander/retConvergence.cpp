#include "retConvergence.hpp"
#include "../../../ast/baseFunc.hpp"
#include "../../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(retConvergence))

    me::retConvergence(baseObj& obj, baseFunc& f, const node& org):
        super(obj, f, org,
            [&](const node& eval) {
                auto& toConverge = (ntype&) getFunc().getType();

                NM_I(" -> retConvergence: %s ==> %s", toConverge.getRet(), eval);
                toConverge.setRet(eval);
                return true;
            }) {}
}
