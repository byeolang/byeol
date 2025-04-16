#include "baseErr.hpp"
#include "../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(baseErr))

    me::baseErr(errLv::level t): super(), _lv(t) { _initStack(); }

    me::baseErr(const me& rhs) { _assign(rhs); }

    me& me::operator=(const me& rhs) {
        WHEN(this == &rhs).ret(*this);
        super::operator=(rhs);

        return _assign(rhs);
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    scope& me::makeSubs() {
        return tbridger<me>::closure<void, me>("log", [&](me& meObj) { meObj.log(); })
            .func("logStack", &me::logStack)
            .closure<const std::string&, me>("getMsg", [&](me& meObj) { return meObj.getMsg(); })
            .subs();
    }

    const callstack& me::getStack() const { return *_stack; }

    void me::logStack() const { _stack->dump(); }

    void me::_initStack() {
        thread& thr = thread::get() orRet;
        _stack.bind(new callstack(thr.getFrames()));
    }

    me& me::_assign(const me& rhs) {
        _stack = rhs._stack;
        _lv = rhs._lv;

        return *this;
    }

    void me::dump() const {
        log();
        logStack();
    }

    errLv::level me::getLv() const { return _lv; }

} // namespace nm
