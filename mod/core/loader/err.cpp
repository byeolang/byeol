#include "../ast/node.hpp"
#include "err.hpp"
#include "../frame/frame.hpp"
#include "../frame/thread.hpp"
#include "../bridge/cpp/tApiBridge.hpp"
#include "../ast/defaultCopyCtor.hpp"

namespace nm {

    NM_DEF_ME(err)

    namespace {
        constexpr nint MAX_BUF = 512;

        class logFunc : public tApiBridge<err, nVoid> {
            typedef tApiBridge<err, nVoid> __super9;
            NM(CLASS(logFunc, __super9))

        public:
            const params& getParams() const override {
                static params inner;
                return inner;
            }

        protected:
            str _onRun(err& cast, const args& a) const override {
                if(a.len() > 0) return str();

                cast.log();
                return str(nVoid::singletone());
            }
        };

        class logStackFunc : public tApiBridge<err, nVoid> {
            typedef tApiBridge<err, nVoid> __super10;
            NM(CLASS(logStackFunc, __super10))

        public:
            const params& getParams() const override {
                static params inner;
                return inner;
            }

        protected:
            str _onRun(err& cast, const args& a) const override {
                if(a.len() > 0) return str();

                cast.logStack();
                return str(nVoid::singletone());
            }
        };
    }

    const err& me::singletone() {
        static me inner(logLv::ERR, errCode::UNKNOWN);
        return inner;
    }

    const std::string& err::getErrMsg(errCode code) {
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

    const std::string& err::getErrName(errCode code) {
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

    err* me::newErr(int code, ...) {
        va_list args;
        va_start(args, code);
        err* ret = newErr(code, args);
        va_end(args);
        return ret;
    }
    err* me::newErr(int code, va_list args) {
        return new err(logLv::ERR, code, args);
    }
    err* me::newErr(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        err* ret = newErr(pos, code, args);
        va_end(args);
        return ret;
    }
    err* me::newErr(const point& pos, int code, va_list args) {
        return new err(logLv::ERR, pos, code, args);
    }
    err* me::newWarn(int code, ...) {
        va_list args;
        va_start(args, code);
        err* ret = newWarn(code, args);
        va_end(args);
        return ret;
    }
    err* me::newWarn(int code, va_list args) {
        return new err(logLv::WARN, code, args);
    }
    err* me::newWarn(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        err* ret = newWarn(pos, code, args);
        va_end(args);
        return ret;
    }
    err* me::newWarn(const point& pos, int code, va_list args) {
        return new err(logLv::WARN, pos, code, args);
    }
    err* me::newInfo(int code, ...) {
        va_list args;
        va_start(args, code);
        err* ret = newInfo(code, args);
        va_end(args);
        return ret;
    }
    err* me::newInfo(int code, va_list args) {
        return new err(logLv::INFO, code, args);
    }
    err* me::newInfo(const point& pos, int code, ...) {
        va_list args;
        va_start(args, code);
        err* ret = newInfo(pos, code, args);
        va_end(args);
        return ret;
    }
    err* me::newInfo(const point& pos, int code, va_list args) {
        return new err(logLv::INFO, pos, code, args);
    }

    me::err(logLv::level t, nint newCode): super(), fType(t), code((errCode) newCode) {
        msg = getErrMsg(code);
        _initStack();
    }

    me::err(logLv::level t, nint newCode, va_list args): super(), fType(t), code((errCode) newCode) {
        msg = _format(getErrMsg(code), args);
        _initStack();
    }

    me::err(logLv::level t, const point& ps, nint newCode, va_list args)
        : super(), fType(t), code((errCode) newCode), pos(ps) {
        msg = _format(getErrMsg(code), args);
        _initStack();
    }

    me::err(const me& rhs): fType(rhs.fType), code(rhs.code), msg(rhs.msg) {
        if(rhs._stack.hasTraces())
            _stack = rhs._stack;
        _initStack();
    }

    nbool me::operator==(const me& rhs) const {
        return fType == rhs.fType && code == rhs.code && code == rhs.code;
    }
    nbool me::operator!=(const me& rhs) const {
        return !operator==(rhs);
    }

    scope& me::subs() {
        static scope inner;
        if(inner.len() <= 0) {
            inner.add("log", new logFunc());
            inner.add("logStack", new logStackFunc());
            inner.add(baseObj::CTOR_NAME, new defaultCtor(singletone()));
            inner.add(baseObj::CTOR_NAME, new defaultCopyCtor(singletone()));
        }
        return inner;
    }

    str me::run(const args& a) {
        tpriorities<baseFunc> p = subAll<baseFunc>(baseObj::CTOR_NAME, a);
        auto matches = p.getMatches();
        switch(matches.len()) {
            case 1: return run(baseObj::CTOR_NAME, a);
            case 0: return NM_W("there is no such ctor."), str();
        }
        /*// TODO: 1. change err management module to use 'err' class, not errCode.
          //       2. let it log all ambigious funcs here.
          return NM_W("")*/
        return NM_E("ambigious call found: %s", "TODO:"), str();
    }

    const baseObj& me::getOrigin() const { return singletone(); }

    void me::log() const {
        using platformAPI::foreColor;
        auto& log = logger::get();
        switch(fType) {
            case logLv::ERR:
                if(pos.isOrigin())
                    log.logFormatBypass("%serr%d(%s)", foreColor(LIGHTRED).c_str(), code, getErrName(code).c_str());
                else
                    log.logFormatBypass("%serr%d(%s) row%d col%d", foreColor(LIGHTRED).c_str(), code, getErrName(code).c_str(),
                                        pos.row, pos.col);
                break;

            case logLv::WARN:
                if(pos.isOrigin())
                    log.logFormatBypass("%swarn%d(%s)", foreColor(YELLOW).c_str(), code, getErrName(code).c_str());
                else
                    log.logFormatBypass("%swarn%d(%s) row%d col%d", foreColor(YELLOW).c_str(), code, getErrName(code).c_str(),
                                        pos.row, pos.col);
                break;

            case logLv::INFO:
                if(pos.isOrigin())
                    log.logFormatBypass("%sinfo%d(%s)", foreColor(BLUE).c_str(), code, getErrName(code).c_str());
                else
                    log.logFormatBypass("%sinfo%d(%s) row%d col%d", foreColor(BLUE).c_str(), code, getErrName(code).c_str(),
                                        pos.row, pos.col);
                break;
        }
        log.logFormatBypass("%s: %s\n", foreColor(LIGHTGRAY).c_str(), msg.c_str());
    }

    void me::dbgLog() const {
        if(buildFeature::config::isDbg())
            log();
    }

    const callstack& me::getStack() const {
        return _stack;
    }

    void me::logStack() const {
        _stack.dump();
        if(buildFeature::config::isDbg())
            _logNativeStack();
        logger::get().logBypass("\n");
    }

    void me::_logNativeStack() const {
        auto native = platformAPI::callstack();
        if(native.size() <= 0) return;

        logger& log = logger::get();
        using platformAPI::foreColor;
        log.logFormatBypass("\t%s----------------------------------%s\n", foreColor(BROWN).c_str(), foreColor(LIGHTGRAY).c_str());
        for(const std::string& trace : native) {
            log.logFormatBypass("\tat %s%s%s\n", foreColor(BLUE).c_str(), trace.c_str(), foreColor(LIGHTGRAY).c_str());
        }
    }

    void me::_initStack() {
        thread& thr = thread::get();
        if(!nul(thr))
            _stack.setStack(thread::get().getFrames());
    }

    void me::dump() const {
        log();
        logStack();
    }

    std::string me::_format(const std::string& fmt, va_list args) {
        nchar buf[MAX_BUF] = {0, };
        vsnprintf(buf, MAX_BUF, fmt.c_str(), args);

        return buf;
    }

    dummyErr::dummyErr(): super(logLv::ERR, 0) {}

    void dummyErr::log() const {}

}
