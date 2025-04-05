#pragma once

#include "exMaker.hpp"

namespace nm {

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __core_when__

    class _nout __WHEN_OBJECT__ : public __clog_when__ {
        NM(ME(__WHEN_OBJECT__, __clog_when__))

    public:
        static const me& get();

        template <typename... Ts>
        void ex(const Ts&... args) const {
            ::nm::exMaker::make(args...);
            return;
        }

        template <typename R, typename... Ts>
        R& ex([[maybe_unused]] R& r, const Ts&... args) const {
            ::nm::exMaker::make(args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& ex([[maybe_unused]] R&& r, const Ts&... args) const {
            ::nm::exMaker::make(args...);
            return std::move(r);
        }

        template <typename... Ts>
        void ex(errReport& rpt, const Ts&... args) const {
            ::nm::exMaker::make(rpt, args...);
            return;
        }

        template <typename R, typename... Ts>
        R& ex([[maybe_unused]] R& r, errReport& rpt, const Ts&... args) const {
            ::nm::exMaker::make(rpt, args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& ex([[maybe_unused]] R&& r, errReport& rpt, const Ts&... args) const {
            ::nm::exMaker::make(rpt, args...);
            return std::move(r);
        }
    };
}
