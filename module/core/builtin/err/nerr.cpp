#include "core/builtin/err/nerr.hpp"

#include "core/ast/ctor/defaultCopyCtor.hpp"
#include "core/ast/node.hpp"
#include "core/frame/frame.hpp"
#include "core/frame/thread.hpp"
#include "core/bridge/cpp/tbridger.hpp"

namespace by {

    BY_DEF_ME(nerr)

    namespace {}

    const me& me::singleton() {
        static me inner(errLv::ERR, errCode::UNKNOWN);
        return inner;
    }

    const std::string& me::getErrMsg(errCode code) {
        static std::string msg[] = {
            "start",
#define PAIRX PAIRX_RIGHT
#include "errCode.inl"
#undef PAIRX
            "end",
        };

        if(code < ERR_CODE_START) code = ERR_CODE_START;
        if(code > ERR_CODE_END) code = ERR_CODE_END;

        return msg[code];
    }

    const std::string& me::getErrName(errCode code) {
        static std::string msg[] = {
            "ERR_CODE_START",
#define PAIRX(A, B) #A,
#include "errCode.inl"
#undef PAIRX
            "ERR_CODE_END",
        };

        if(code < ERR_CODE_START) code = ERR_CODE_START;
        if(code > ERR_CODE_END) code = ERR_CODE_END;

        return msg[code];
    }

    me* me::newErr(int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newErr(code, args);
        va_end(args);
        return ret;
    }

    me* me::newErr(int code, va_list args) { return new nerr(errLv::ERR, code, args); }

    me* me::newErr(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = new me(errLv::ERR, pos, code, args);
        va_end(args);
        return ret;
    }

    me* me::newErr(const point& pos, int code, va_list args) { return new me(errLv::ERR, pos, code, args); }

    me* me::newWarn(int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newWarn(code, args);
        va_end(args);
        return ret;
    }

    me* me::newWarn(int code, va_list args) { return new me(errLv::WARN, code, args); }

    me* me::newWarn(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newWarn(pos, code, args);
        va_end(args);
        return ret;
    }

    me* me::newWarn(const point& pos, int code, va_list args) { return new me(errLv::WARN, pos, code, args); }

    me* me::newInfo(int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newInfo(code, args);
        va_end(args);
        return ret;
    }

    me* me::newInfo(int code, va_list args) { return new me(errLv::INFO, code, args); }

    me* me::newInfo(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newInfo(pos, code, args);
        va_end(args);
        return ret;
    }

    me* me::newInfo(const point& pos, int code, va_list args) { return new me(errLv::INFO, pos, code, args); }

    me::nerr(errLv::level t, nint newCode): super(t), _code((errCode) newCode), _pos{}, _msg(getErrMsg(_code)) {}

    me::nerr(errLv::level t, nint newCode, va_list args):
        super(t), _code((errCode) newCode), _pos{}, _msg(platformAPI::format(getErrMsg(_code), args)) {}

    me::nerr(errLv::level t, const point& ps, nint newCode, va_list args):
        super(t), _code((errCode) newCode), _pos(ps), _msg(platformAPI::format(getErrMsg(_code), args)) {}

    me::nerr(const me& rhs): super(rhs), _code(rhs._code), _pos(rhs._pos), _msg(rhs._msg) {}

    me::nerr(): super(errLv::ERR) {}

    nbool me::operator==(const super& rhs) const {
        const me& cast = rhs.cast<me>() OR.ret(false);

        return getLv() == cast.getLv() && _code == cast.getErrCode();
    }

    const baseObj& me::getOrigin() const {
        static tbaseObjOrigin<me> org(me::super::makeSubs());
        const baseObj& supers = super::getOrigin();
        return &supers == this ? org : supers;
    }

    void me::log() const {
        using platformAPI::foreColor;
        auto& log = logger::get();
        switch(getLv()) {
            case errLv::ERR:
                if(_pos.isOrigin())
                    log.logFormatBypass("%serr%d(%s)", foreColor(LIGHTRED).c_str(), _code, getErrName(_code).c_str());
                else
                    log.logFormatBypass("%serr%d(%s) row%d col%d", foreColor(LIGHTRED).c_str(), _code,
                        getErrName(_code).c_str(), _pos.row, _pos.col);
                break;

            case errLv::WARN:
                if(_pos.isOrigin())
                    log.logFormatBypass("%swarn%d(%s)", foreColor(YELLOW).c_str(), _code, getErrName(_code).c_str());
                else
                    log.logFormatBypass("%swarn%d(%s) row%d col%d", foreColor(YELLOW).c_str(), _code,
                        getErrName(_code).c_str(), _pos.row, _pos.col);
                break;

            case errLv::INFO:
                if(_pos.isOrigin())
                    log.logFormatBypass("%sinfo%d(%s)", foreColor(BLUE).c_str(), _code, getErrName(_code).c_str());
                else
                    log.logFormatBypass("%sinfo%d(%s) row%d col%d", foreColor(BLUE).c_str(), _code,
                        getErrName(_code).c_str(), _pos.row, _pos.col);
                break;
        }
        log.logFormatBypass("%s: %s\n", foreColor(LIGHTGRAY).c_str(), getMsg().c_str());
    }

    const std::string& me::getMsg() const { return _msg; }

    errCode me::getErrCode() const { return _code; }

    const point& me::getPos() const { return _pos; }

    void me::logStack() const {
        super::logStack();
        if(buildFeature::config::isDbg()) _logNativeStack();
        logger::get().logBypass("\n");
    }

    void me::_logNativeStack() const {
        auto native = platformAPI::callstack();
        WHEN(native.size() <= 0) .ret();

        logger& log = logger::get();
        using platformAPI::foreColor;
        log.logFormatBypass("\t%s----------------------------------%s\n", foreColor(BROWN).c_str(),
            foreColor(LIGHTGRAY).c_str());
        for(const std::string& trace: native)
            log.logFormatBypass("\tat %s%s%s\n", foreColor(BLUE).c_str(), trace.c_str(), foreColor(LIGHTGRAY).c_str());
    }

    ndummyErr::ndummyErr(): super(errLv::ERR, 0) {}

    void ndummyErr::log() const {}

    const std::string& ndummyErr::getMsg() const {
        static std::string inner = "";
        return inner;
    }
} // namespace by
