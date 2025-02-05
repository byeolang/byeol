#include "paramConvergence.hpp"
#include "../../../../ast/param.hpp"
#include "../../../../ast/func.hpp"
#include "../../../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(paramConvergence))

    me::paramConvergence(baseObj& o, baseFunc& f, param& p, const node& org):
        super(o, f, org,
            [&](const node& eval) {
                _p->setOrigin(eval);
                return true;
            }),
        _p(&p) {}
}
