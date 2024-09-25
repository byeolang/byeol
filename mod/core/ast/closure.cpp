#include "closure.hpp"
#include "../frame/thread.hpp"
#include "../frame/frameInteract.hpp"
#include "../worker/visitor/visitor.hpp"
#include "exprs/getExpr.hpp"
#include "baseFunc.hpp"

namespace nm {
    NM(DEF_ME(closure), DEF_VISIT())

    me::closure(const node& capture, const baseFunc& func): _capture(capture), _func(func) {}

    scope& me::subs() { return _func->subs(); }

    priorType me::prioritize(const args& a) const { return _func->prioritize(a); }

    const ntype& me::getType() const { return _func->getType(); }

    const modifier& me::getModifier() const { return _func->getModifier(); }

    str me::run(const args& a) {
        NM_I("running closure for %s.%s", *_capture, *_func);
        a.setMe(*_capture);
        return _func->run(a);
    }

    node& me::getCapture() { return *_capture; }

    baseFunc& me::getFunc() { return *_func; }
} // namespace nm
