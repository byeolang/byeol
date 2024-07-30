#pragma once

#include <unordered_map>

#include "../ast/point.hpp"
#include "../builtin/container/native/tnbicontainer.hpp"
#include "../common/namuMetaExtension.hpp"
#include "../frame/callstack.hpp"
#include "../frame/frame.hpp"
#include "../type/dumpable.hpp"
#include "../type/ntype.hpp"
#include "errCode.hpp"

namespace nm {

    typedef std::unordered_map<nidx, std::string> msgMap;

    class pos;

    struct _nout err: public baseObj, public dumpable {
        NM(CLASS(err, baseObj))
        template <typename T, nbool> friend struct tmarshaling;

    public:
        err(logLv::level t, nint newCode);
        err(logLv::level t, nint newCode, va_list args);
        err(logLv::level t, const point& ps, nint newCode, va_list args);
        err(const me& rhs);

    private:
        /// @hidden this's only available to marsharling.
        err();

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        using super::subs;
        scope& subs() override;
        using super::run;
        str run(const args& a) override;

        const baseObj& getOrigin() const override;

        virtual void log() const;
        void dbgLog() const;

        const callstack& getStack() const;
        void logStack() const;

        static const err& singletone();
        static const std::string& getErrMsg(errCode code);
        static const std::string& getErrName(errCode code);
        static err* newErr(int code, ...);
        static err* newErr(int code, va_list args);
        static err* newErr(const point& src, int code, ...);
        static err* newErr(const point& src, int code, va_list args);
        static err* newWarn(int code, ...);
        static err* newWarn(int code, va_list args);
        static err* newWarn(const point& src, int code, ...);
        static err* newWarn(const point& src, int code, va_list args);
        static err* newInfo(int code, ...);
        static err* newInfo(int code, va_list args);
        static err* newInfo(const point& src, int code, ...);
        static err* newInfo(const point& src, int code, va_list args);

        void dump() const override;

    private:
        std::string _format(const std::string& fmt, va_list args);
        void _logNativeStack() const;
        void _initStack();

    public:
        logLv::level fType;
        errCode code;
        std::string msg;
        point pos;
        callstack _stack;
        static constexpr nint BASE_TEST_CODE = 99999990; // not to be duplicated.
    };

    struct _nout dummyErr: public err {
        NM(CLASS(dummyErr, err))

    public:
        dummyErr();

    public:
        void log() const override;
    };
} // namespace nm
