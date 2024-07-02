#include "signaler.hpp"
#include <csignal>

namespace nm {

    nbool sig::operator==(const me& rhs) const {
        return code == rhs.code;
    }

    nbool sig::operator!=(const me& rhs) const {
        return !operator==(rhs);
    }

    NAMU(DEF_ME(signaler))

    namespace {
        const sig signals[] = {
            {SIGSEGV, [](nint code) -> err* { return new err(logLv::ERR, errCode::SIG_SEGV); }},
            {SIGINT, [](nint code) -> err* { return new err(logLv::ERR, errCode::SIG_INT); }},
            {SIGILL, [](nint code) -> err* { return new err(logLv::ERR, errCode::SIG_ILL); }},
            {SIGABRT, [](nint code) -> err* { return new err(logLv::ERR, errCode::SIG_ABORT); }},
            {SIGFPE, [](nint code) -> err* { return new err(logLv::ERR, errCode::SIG_FPE); }},
            {SIGTERM, [](nint code) -> err* { return new err(logLv::ERR, errCode::SIG_TERM); }},
        };

        void _onSignal(nint code) {
            NAMU_E("# signal %d caught.", code);
            me::get().onSignal(code);
        }
    }

    me::signaler() {}
    signaler& me::get() {
        static me inner;
        return inner;
    }

    void me::addSignal(sigHandler closure) {
        if(_closures.size() <= 0)
            _setSignal(_onSignal);
        _closures.push_back(closure);
        NAMU_DI("total %d signal handler planted.", _closures.size());
    }

    void me::onSignal(nint code) {
        _setSignal(SIG_DFL); // prevent infinite loop if another signal occurs during handling the signal here.

        tstr<err> e(_getErrBy(code));
        if(!e) return NAMU_E("%d exception occurs but couldn't make err object", code), void();

        NAMU_I("dispatching %d handlers.", _closures.size());
        for(sigHandler handler : _closures)
            handler(*e);

        _closures.clear();
        exit(EXIT_FAILURE);
    }

    namespace {
        void* _getAddr(sigHandler handler) {
            return (void*) handler.target<sigFunc>();
        }
    }

    void me::delSignal(sigHandler closure) {
        void* closureAddr = _getAddr(closure);
        _closures.erase(std::remove_if(_closures.begin(), _closures.end(), [&](sigHandler elem) { return _getAddr(elem) == closureAddr; }), _closures.end());
        if(_closures.size() <= 0)
            _setSignal(SIG_DFL);
        NAMU_DI("signal handler deleted. total %d handlers remains", _closures.size());
    }

    void me::_setSignal(void(*csignalHandler)(nint)) {
        for(const sig& s : signals)
            signal(s.code, csignalHandler);
    }

    void me::relSignal() {
        _setSignal(SIG_DFL);
        _closures.clear();
    }

    const err* me::_getErrBy(nint code) const {
        for(const sig& e : signals)
            if(e.code == code) return e.maker(code);
        return nullptr;
    }
}
