#include "baseErr.hpp"
#include "frame/thread.hpp"

namespace nm {
    NM(DEF_ME(baseErr))

    me::baseErr() {}

    me::baseErr(logLv::level t): fType(t) { _initStack(); }

    me::baseErr(const me& rhs) {
        if(rhs._stack.hasTraces()) _stack = rhs._stack;
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

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

    const callstack& me::getStack() const { return _stack; }

    void me::logStack() const { _stack.dump(); }

    void me::_initStack() {
        thread& thr = thread::get();
        if(!nul(thr)) _stack.setStack(thread::get().getFrames());
    }

    void me::dump() const {
        log();
        logStack();
    }

    const std::string& me::getLevelName() const {
        return logLv::getName(fType);
    }
} // namespace nm
