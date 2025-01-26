#include "retConvergence.hpp"
#include "../../../ast/baseFunc.hpp"
#include "../../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(retConvergence))

    me::retConvergence(baseObj& obj, baseFunc& f, const node& org):
        super(obj, f,
            [&]() {
                str eval = _org->getEval() orRet false;
                const node& owner = getFunc() THEN(getOrigin()) orRet false;
                const frame& fr = thread::get().getNowFrame();
                if(&fr.getMeHaving(*eval) != &owner) return false;

                NM_I(" -> retConvergence: %s ==> %s", getFunc().getType().getRet(), *eval);
                ((ntype&) getFunc().getType()).setRet(*eval);
                return true;
            }),
        _org(org) {}
}
