#include "convergence.hpp"
#include "../../../frame/frameInteract.hpp"
#include "../../../ast/baseFunc.hpp"

namespace nm {
    NM(DEF_ME(convergence))

    me::convergence(baseObj& obj, baseFunc& func): _obj(obj), _func(func) {}

    nbool me::converge() {
        if(!_obj) return false;
        baseFunc& fun = *_func orRet false;

        frameInteract objScope(*_obj);
        {
            frameInteract funScope(fun);
            { return _onConverge(fun); }
        }
    }
}
