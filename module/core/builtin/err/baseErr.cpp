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

    const frame& me::getFrame() const { return *_frame; }

    void me::logStack() const { _frame->dump(); }

    void me::_initStack() {
        frame& last = thread::get().getFrames().last().get() OR.ret();
        _frame.bind((frame*) last.clone());
    }

    me& me::_assign(const me& rhs) {
        _frame = rhs._frame;
        _lv = rhs._lv;

        return *this;
    }

    void me::dump() const {
        log();
        logStack();
    }

    errLv::level me::getLv() const { return _lv; }

} // namespace by
