#include "core/frame/signaler.hpp"

#include <csignal>
#include "core/builtin/err/nerr.hpp"

namespace by {

    nbool sig::operator==(const me& rhs) const { return code == rhs.code; }

    nbool sig::operator!=(const me& rhs) const { return !operator==(rhs); }

    BY(DEF_ME(signaler))

    namespace {
        const sig signals[] = {
            {SIGSEGV,
             [](nint code) -> nerr* {
                    return new nerr(errLv::ERR, errCode::SIG_SEGV);
                }},
            {SIGINT,
             [](nint code) -> nerr* {
                    return new nerr(errLv::ERR, errCode::SIG_INT);
                }},
            {SIGILL,
             [](nint code) -> nerr* {
                    return new nerr(errLv::ERR, errCode::SIG_ILL);
                }},
            {SIGABRT,
             [](nint code) -> nerr* {
                    return new nerr(errLv::ERR, errCode::SIG_ABORT);
                }},
            {SIGFPE,
             [](nint code) -> nerr* {
                    return new nerr(errLv::ERR, errCode::SIG_FPE);
                }},
            {SIGTERM,
             [](nint code) -> nerr* {
                    return new nerr(errLv::ERR, errCode::SIG_TERM);
                }},
        };

        void _onSignal(nint code) {
            BY_E("# signal %d caught.", code);
            me::get().onSignal(code);
        }
    }

    me::signaler() {}

    signaler& me::get() {
        static me inner;
        return inner;
    }

    void me::addSignal(const sigHandler& closure) {
        if(_closures.size() <= 0) _setSignal(_onSignal);
        _closures.push_back(closure);
        BY_DI("total %d signal handler planted.", _closures.size());
    }

    void me::onSignal(nint code) {
        _setSignal(SIG_DFL); // prevent infinite loop if another signal occurs during handling the
                             // signal here.

        tstr<nerr> e = _getErrBy(code) OR.err("%d exception occurs but couldn't make err object", code).ret();

        BY_I("dispatching %d handlers.", _closures.size());
        for(const sigHandler& handler: _closures)
            handler(*e);

        _closures.clear();
        exit(EXIT_FAILURE);
    }

    namespace {
        void* _getAddr(sigHandler handler) { return (void*) handler.target<sigFunc>(); }
    }

    void me::delSignal(const sigHandler& closure) {
        void* closureAddr = _getAddr(closure);
        _closures.erase(std::remove_if(_closures.begin(), _closures.end(),
                            [&](sigHandler elem) { return _getAddr(std::move(elem)) == closureAddr; }),
            _closures.end());

        if(_closures.size() <= 0) _setSignal(SIG_DFL);
        BY_DI("signal handler deleted. total %d handlers remains", _closures.size());
    }

    void me::_setSignal(void (*csignalHandler)(nint)) {
        for(const sig& s: signals)
            signal(s.code, csignalHandler);
    }

    void me::relSignal() {
        _setSignal(SIG_DFL);
        _closures.clear();
    }

    const nerr* me::_getErrBy(nint code) const {
        for(const sig& e: signals)
            WHEN(e.code == code) .ret(e.maker(code));
        return nullptr;
    }
} // namespace by
