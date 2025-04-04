#pragma once

#include "richLog.hpp"

namespace nm {

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __clog_when__

    class _nout __WHEN_OBJECT__ : public __indep_when__ {
        NM(ME(__WHEN_OBJECT__))

    public:
        static const me& get();

        template <typename... Ts>
        void err(const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::ERR, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return;
        }

        template <typename R, typename... Ts>
        R& err([[maybe_unused]] R& r, const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::ERR, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& err([[maybe_unused]] R&& r, const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::ERR, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return std::move(r);
        }

        template <typename... Ts>
        void warn(const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::WARN, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return;
        }

        template <typename R, typename... Ts>
        R& warn([[maybe_unused]] R& r, const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::WARN, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& warn([[maybe_unused]] R&& r, const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::WARN, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return std::move(r);
        }

        template <typename... Ts>
        void info(const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::INFO, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return;
        }

        template <typename R, typename... Ts>
        R& info([[maybe_unused]] R& r, const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::INFO, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& info([[maybe_unused]] R&& r, const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::logLv::INFO, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return std::move(r);
        }
    };
} // namespace nm
