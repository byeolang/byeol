#pragma once

#include "clog/richLog.hpp"

namespace nm {

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __clog_when__

    class _nout __WHEN_OBJECT__: public __indep_when__ {
        NM(ME(__WHEN_OBJECT__))

    public:
        static const me& get();

        template <typename... Ts> const me& err(const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::errLv::ERR, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return *this;
        }

        template <typename... Ts> const me& warn(const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::errLv::WARN, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return *this;
        }

        template <typename... Ts> const me& info(const nchar* fmt, const Ts&... args) const {
            ::nm::richLog(::nm::errLv::INFO, __FILENAME__, __func__, __LINE__,
                (std::string(fmt) + "\n").c_str(), args...);
            return *this;
        }
    };
} // namespace nm
