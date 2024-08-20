#include "baseErr.hpp"
#include "../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(baseErr))

    me::baseErr(): _lv(logLv::ERR) { _initStack(); }

    me::baseErr(logLv::level t): _lv(t) { _initStack(); }

    me::baseErr(const me& rhs) { _assign(rhs); }

    me& me::operator=(const me& rhs) {
        if(this == &rhs) return *this;
        super::operator=(rhs);

        return _assign(rhs);
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    scope& me::subs() {
        static scope inner = tbridger<me>::func("log", &me::log)
                                 .func("logStack", &me::logStack)
                                 .func("getMsg", &me::getMsg)
                                 .subs();
        return inner;
    }

    str me::run(const args& a) {
        tpriorities<baseFunc> matches = subAll<baseFunc>(baseObj::CTOR_NAME, a);
        switch(matches.len()) {
            case 1: return run(baseObj::CTOR_NAME, a);
            case 0: return NM_W("there is no such ctor."), str();
        }
        /*// TODO: 1. change err management module to use 'err' class, not errCode.
          //       2. let it log all ambigious funcs here.
          return NM_W("")*/
        return NM_E("ambigious call found: %s", "TODO:"), str();
    }

    void me::dbgLog() const {
        if(buildFeature::config::isDbg()) log();
    }

    const callstack& me::getStack() const { return *_stack; }

    void me::logStack() const { _stack->dump(); }

    void me::_initStack() {
        thread& thr = thread::get();
        if(nul(thr)) return;

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

    const std::string& me::getLevelName() const { return logLv::getName(_lv); }

    logLv::level me::getLv() const { return _lv; }

} // namespace nm
