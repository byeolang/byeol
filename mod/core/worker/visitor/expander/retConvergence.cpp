#include "retConvergence.hpp"
#include "../../../ast/baseFunc.hpp"
#include "../../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(retConvergence))

    me::retConvergence(baseFunc& f, const node& org): _f(&f), _org(&org) {}

    nbool me::converge() const {
        if(!_f || !_org) return NM_E("some retConverge info is null"), false;

        str eval = _org->getEval() orRet false;
        const node& owner = _f->getOrigin() orRet false;
        const frame& fr = thread::get().getNowFrame();
        if(&fr.getOwner(*eval) != &owner) return false;

        NM_I(" -> retConvergence: %s ==> %s", _f->getType().getRet(), *eval);
        ((ntype&) _f->getType()).setRet(*eval);
        return true;
    }
}
