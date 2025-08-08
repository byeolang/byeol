#pragma once

#include <unordered_map>

#include "core/builtin/err/baseErr.hpp"
#include "core/builtin/container/native/tnbicontainer.hpp"
#include "core/common/byeolMetaExtension.hpp"
#include "core/type/ntype.hpp"
#include "core/builtin/err/errCode.hpp"

namespace by {

    typedef std::unordered_map<nidx, std::string> msgMap;

    class pos;

    struct _nout nerr: public baseErr {
        NM(CLASS(nerr, baseErr))
        template <typename T, nbool> friend struct tmarshaling;

    public:
        nerr(errLv::level t, nint newCode);
        nerr(errLv::level t, nint newCode, va_list args);
        nerr(errLv::level t, const point& ps, nint newCode, va_list args);
        nerr(const me& rhs);

    protected:
        /// @hidden this's only available to marsharling.
        nerr();

    public:
        nbool operator==(const super& rhs) const override;

    public:
        const baseObj& getOrigin() const override;

        void log() const override;
        void logStack() const override;
        const std::string& getMsg() const override;
        errCode getErrCode() const;
        const point& getPos() const;

        static const nerr& singleton();
        static const std::string& getErrMsg(errCode code);
        static const std::string& getErrName(errCode code);
        static nerr* newErr(int code, ...);
        static nerr* newErr(int code, va_list args);
        static nerr* newErr(const point& src, int code, ...);
        static nerr* newErr(const point& src, int code, va_list args);
        static nerr* newWarn(int code, ...);
        static nerr* newWarn(int code, va_list args);
        static nerr* newWarn(const point& src, int code, ...);
        static nerr* newWarn(const point& src, int code, va_list args);
        static nerr* newInfo(int code, ...);
        static nerr* newInfo(int code, va_list args);
        static nerr* newInfo(const point& src, int code, ...);
        static nerr* newInfo(const point& src, int code, va_list args);

    private:
        void _logNativeStack() const;

    private:
        errCode _code;
        point _pos;
        std::string _msg;
    };

    struct _nout ndummyErr: public nerr {
        NM(CLASS(ndummyErr, nerr))

    public:
        ndummyErr();

    public:
        void log() const override;
        const std::string& getMsg() const override;
    };
} // namespace by
