#include "convergence.hpp"
#include "../../../frame/frameInteract.hpp"
#include "../../../ast/baseFunc.hpp"

namespace nm {
    NM(DEF_ME(convergence))

    me::convergence(baseObj& obj, baseFunc& func, std::function<nbool()> closure): _obj(obj), _func(func), _cl(closure) {}

    nbool me::converge() {
        if(!_obj) return false;
        baseFunc& fun = *_func orRet false;

        frameInteract objScope(*_obj);
        {
            frameInteract funScope(fun);
            { return _cl(); }
        }
    }

    std::function<nbool()> me::getClosure() { return _cl; }

    baseObj& me::getObj() { return *_obj; }

    baseFunc& me::getFunc() { return *_func; }
}
