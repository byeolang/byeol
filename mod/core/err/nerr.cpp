#include "nerr.hpp"

#include "../ast/ctor/defaultCopyCtor.hpp"
#include "../ast/node.hpp"
#include "../frame/frame.hpp"
#include "../frame/thread.hpp"
#include "bridge/cpp/tbridger.hpp"

namespace nm {

    NM_DEF_ME(nerr)

    namespace {
        constexpr nint MAX_BUF = 512;
    }

    const me& me::singletone() {
        static me inner(logLv::ERR, errCode::UNKNOWN);
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

    me* me::newErr(int code, va_list args) { return new me(logLv::ERR, code, args); }

    me* me::newErr(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newErr(pos, code, args);
        va_end(args);
        return ret;
    }

    me* me::newErr(const point& pos, int code, va_list args) {
        return new me(logLv::ERR, pos, code, args);
    }

    me* me::newWarn(int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newWarn(code, args);
        va_end(args);
        return ret;
    }

    me* me::newWarn(int code, va_list args) { return new me(logLv::WARN, code, args); }

    me* me::newWarn(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newWarn(pos, code, args);
        va_end(args);
        return ret;
    }

    me* me::newWarn(const point& pos, int code, va_list args) {
        return new me(logLv::WARN, pos, code, args);
    }

    me* me::newInfo(int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newInfo(code, args);
        va_end(args);
        return ret;
    }

    me* me::newInfo(int code, va_list args) { return new me(logLv::INFO, code, args); }

    me* me::newInfo(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        me* ret = newInfo(pos, code, args);
        va_end(args);
        return ret;
    }

    me* me::newInfo(const point& pos, int code, va_list args) {
        return new me(logLv::INFO, pos, code, args);
    }

    me::nerr(logLv::level t, nint newCode):
        super(t), code((errCode) newCode), msg(getErrMsg(code)) {}

    me::nerr(logLv::level t, nint newCode, va_list args):
        super(t), code((errCode) newCode), msg(_format(getErrMsg(code), args)) {}

    me::nerr(logLv::level t, const point& ps, nint newCode, va_list args):
        super(t, ps), code((errCode) newCode), msg(_format(getErrMsg(code), args)) {}

    me::nerr(const me& rhs): super(rhs), code(rhs.code), msg(rhs.msg) {}

    me::nerr(): super() {}

    nbool me::operator==(const super& rhs) const {
        const me& cast = rhs.cast<me>();
        if(nul(cast)) return false;

        return getLv() == cast.getLv() && code == cast.code && code == cast.code;
    }

    scope& me::subs() {
        static scope inner =
            tbridger<super>::func("log", &super::log).func("logStack", &super::logStack).subs();

        return inner;
    }

    const baseObj& me::getOrigin() const { return singletone(); }

    void me::log() const {
        using platformAPI::foreColor;
        auto& log = logger::get();
        const point& pos = getPos();
        switch(getLv()) {
            case logLv::ERR:
                if(pos.isOrigin())
                    log.logFormatBypass("%serr%d(%s)", foreColor(LIGHTRED).c_str(), code,
                        getErrName(code).c_str());
                else
                    log.logFormatBypass("%serr%d(%s) row%d col%d", foreColor(LIGHTRED).c_str(),
                        code, getErrName(code).c_str(), pos.row, pos.col);
                break;

            case logLv::WARN:
                if(pos.isOrigin())
                    log.logFormatBypass("%swarn%d(%s)", foreColor(YELLOW).c_str(), code,
                        getErrName(code).c_str());
                else
                    log.logFormatBypass("%swarn%d(%s) row%d col%d", foreColor(YELLOW).c_str(), code,
                        getErrName(code).c_str(), pos.row, pos.col);
                break;

            case logLv::INFO:
                if(pos.isOrigin())
                    log.logFormatBypass("%sinfo%d(%s)", foreColor(BLUE).c_str(), code,
                        getErrName(code).c_str());
                else
                    log.logFormatBypass("%sinfo%d(%s) row%d col%d", foreColor(BLUE).c_str(), code,
                        getErrName(code).c_str(), pos.row, pos.col);
                break;
        }
        log.logFormatBypass("%s: %s\n", foreColor(LIGHTGRAY).c_str(), msg.c_str());
    }

    std::string me::getMsg() const {
        std::stringstream ss;
        ss << getLevelName() << code << "(" << getErrName(code) << ")";

        const point& pos = getPos();
        if(!pos.isOrigin()) ss << " row" << pos.row << " col" << pos.col << ": " << msg;
        return ss.str();
    }

    void me::logStack() const {
        super::logStack();
        if(buildFeature::config::isDbg()) _logNativeStack();
        logger::get().logBypass("\n");
    }

    void me::_logNativeStack() const {
        auto native = platformAPI::callstack();
        if(native.size() <= 0) return;

        logger& log = logger::get();
        using platformAPI::foreColor;
        log.logFormatBypass("\t%s----------------------------------%s\n", foreColor(BROWN).c_str(),
            foreColor(LIGHTGRAY).c_str());
        for(const std::string& trace: native) {
            log.logFormatBypass("\tat %s%s%s\n", foreColor(BLUE).c_str(), trace.c_str(),
                foreColor(LIGHTGRAY).c_str());
        }
    }

    std::string me::_format(const std::string& fmt, va_list args) {
        nchar buf[MAX_BUF] = {
            0,
        };
        vsnprintf(buf, MAX_BUF, fmt.c_str(), args);

        return buf;
    }

    ndummyErr::ndummyErr(): super(logLv::ERR, 0) {}

    void ndummyErr::log() const {}

    std::string ndummyErr::getMsg() const { return ""; }
} // namespace nm
