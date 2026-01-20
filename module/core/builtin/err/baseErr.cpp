#include "core/builtin/err/baseErr.hpp"
#include "core/frame/thread.hpp"

namespace by {
    BY(DEF_ME(baseErr))

    me::baseErr(errLv::level t): super(), _lv(t) { _initStack(); }

    me::baseErr(const me& rhs): super(rhs) { _assign(rhs); }

    me& me::operator=(const me& rhs) {
        WHEN(this == &rhs) .ret(*this);
        super::operator=(rhs);

        return _assign(rhs);
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    scope& me::makeSubs() {
        return tbridger<me>::closure<void, me>("log", [&](me& meObj) { meObj.log(); })
            .func("logStack", &me::logStack)
            .closure<const std::string&, me>("getMsg", [&](me& meObj) -> const std::string& { return meObj.getMsg(); })
            .subs();
    }

    const std::string& me::getMsg() const {
        static std::string dummy{};
        return dummy;
    }

    errCode me::getErrCode() const { return NOT_SPECIFIED; }

    const frames& me::getFrames() const { return *_frames; }

    void me::logStack() const { _frames->dump(); }

    void me::_initStack() { _frames.bind((frames*) thread::get().getFrames().cloneDeep()); }

    me& me::_assign(const me& rhs) {
        _frames = rhs._frames;
        _lv = rhs._lv;

        return *this;
    }

    void me::dump() const {
        log();
        logStack();
    }

    errLv::level me::getLv() const { return _lv; }

} // namespace by
