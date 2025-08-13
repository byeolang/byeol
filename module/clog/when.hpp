#pragma once

#include "clog/richLog.hpp"

namespace by {

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __clog_when__

    class _nout __WHEN_OBJECT__: public __indep_when__ {
        BY(ME(__WHEN_OBJECT__))

    public:
        static const me& get();

        template <typename... Ts> const me& err(const nchar* fmt, const Ts&... args) const {
            ::by::richLog(::by::errLv::ERR, __FILENAME__, __func__, __LINE__, (std::string(fmt) + "\n").c_str(),
                args...);
            return *this;
        }

        template <typename... Ts> const me& warn(const nchar* fmt, const Ts&... args) const {
            ::by::richLog(::by::errLv::WARN, __FILENAME__, __func__, __LINE__, (std::string(fmt) + "\n").c_str(),
                args...);
            return *this;
        }

        template <typename... Ts> const me& info(const nchar* fmt, const Ts&... args) const {
            ::by::richLog(::by::errLv::INFO, __FILENAME__, __func__, __LINE__, (std::string(fmt) + "\n").c_str(),
                args...);
            return *this;
        }

        template <typename... Ts> const me& dbgErr(const nchar* fmt, const Ts&... args) const {
            ::by::dbgRichLog(::by::errLv::ERR, __FILENAME__, __func__, __LINE__, (std::string(fmt) + "\n").c_str(),
                args...);
            return *this;
        }

        template <typename... Ts> const me& dbgWarn(const nchar* fmt, const Ts&... args) const {
            ::by::dbgRichLog(::by::errLv::WARN, __FILENAME__, __func__, __LINE__, (std::string(fmt) + "\n").c_str(),
                args...);
            return *this;
        }

        template <typename... Ts> const me& dbgInfo(const nchar* fmt, const Ts&... args) const {
            ::by::dbgRichLog(::by::errLv::INFO, __FILENAME__, __func__, __LINE__, (std::string(fmt) + "\n").c_str(),
                args...);
            return *this;
        }
    };
} // namespace by
