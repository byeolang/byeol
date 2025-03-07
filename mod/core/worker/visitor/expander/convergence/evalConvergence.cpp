#include "evalConvergence.hpp"

namespace nm {
    NM(DEF_ME(evalConvergence))

    me::evalConvergence(baseObj& obj, baseFunc& fun, const node& org, onConverge closure):
        super(obj, fun, std::move(closure)), _org(org) {}

    nbool me::convergeWithoutFrame() {
        str eval = _org->getEval() orRet false;
        baseObj& cast = eval->cast<baseObj>();
        if(!nul(cast)) eval.bind(cast.getOrigin());

        return _getClosure()(*eval);
    }
}
