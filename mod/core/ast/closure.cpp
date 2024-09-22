#include "closure.hpp"
#include "../frame/thread.hpp"
#include "../frame/frameInteract.hpp"
#include "../worker/visitor/visitor.hpp"
#include "exprs/getExpr.hpp"
#include "baseFunc.hpp"

namespace nm {
    NM(DEF_ME(closure), DEF_VISIT())

    me::closure(const scope& captured, const baseFunc& func): _capture(captured), _func(func) {}

    scope& me::subs() { return _func->subs(); }

    priorType me::prioritize(const args& a) const { return _func->prioritize(a); }

    const ntype& me::getType() const { return _func->getType(); }

    const modifier& me::getModifier() const { return _func->getModifier(); }

    str me::run(const args& a) {
        frameInteract f1(*this);
        { return _func->run(a); }
    }

    void me::inFrame(const bicontainable& args) {
        frame* fr = new frame();
        fr->add(*_capture);
        thread::get()._getFrames().add(fr);
    }

    void me::outFrame(const bicontainable& args) { thread::get()._getFrames().del(); }

    scope& me::getCaptured() { return *_capture; }

    baseFunc& me::getFunc() { return *_func; }
} // namespace nm
