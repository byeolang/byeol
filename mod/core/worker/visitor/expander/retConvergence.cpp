#include "retConvergence.hpp"
#include "../../../ast/baseFunc.hpp"
#include "../../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(retConvergence))

    me::retConvergence(baseObj& obj, baseFunc& f, const node& org): super(obj, f), _org(&org) {}

    nbool me::_onConverge(baseFunc& f) const {
        str eval = _org->getEval() orRet false;
        const node& owner = f.getOrigin() orRet false;
        const frame& fr = thread::get().getNowFrame();
        if(&fr.getOwner(*eval) != &owner) return false;

        NM_I(" -> retConvergence: %s ==> %s", f.getType().getRet(), *eval);
        ((ntype&) f.getType()).setRet(*eval);
        return true;
    }
}
