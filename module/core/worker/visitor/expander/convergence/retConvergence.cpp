#include "core/worker/visitor/expander/convergence/retConvergence.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/frame/thread.hpp"

namespace by {
    BY(DEF_ME(retConvergence))

    me::retConvergence(baseObj& obj, baseFunc& f, const node& org):
        super(obj, f, org, [&](const node& eval) {
            auto& toConverge = (ntype&) getFunc().getType();

            BY_I(" -> retConvergence: %s ==> %s", toConverge.getRet(), eval);
            toConverge.setRet(eval);
            return true;
        }) {}
}
