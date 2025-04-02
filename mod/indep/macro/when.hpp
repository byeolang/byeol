#pragma once

#include <utility>
#include "../def/_nout.hpp"
#include "common/typedef.hpp"
#include "../common/typedef.hpp"
#include "declThis.hpp"
#include "namuMeta.hpp"

namespace nm {
#define __WHEN_OBJECT__ __basic_when__

    class _nout __WHEN_OBJECT__ {
        NM(ME(__WHEN_OBJECT__))

    public:
        static const me& get();

        template <typename R, typename... Ts>
        R& err([[maybe_unused]] R& r, const nchar* fmt, const Ts&... args) const {
            NM_E(fmt, args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& err([[maybe_unused]] R&& r, const nchar* fmt, const Ts&... args) const {
            NM_E(fmt, args...);
            return std::move(r);
        }

        template <typename R, typename... Ts>
        R& warn([[maybe_unused]] R& r, const nchar* fmt, const Ts&... args) const {
            NM_W(fmt, args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& warn([[maybe_unused]] R&& r, const nchar* fmt, const Ts&... args) const {
            NM_W(fmt, args...);
            return std::move(r);
        }

        template <typename R, typename... Ts>
        R& info([[maybe_unused]] R& r, const nchar* fmt, const Ts&... args) const {
            NM_I(fmt, args...);
            return r;
        }

        template <typename R, typename... Ts>
        R&& info([[maybe_unused]] R&& r, const nchar* fmt, const Ts&... args) const {
            NM_I(fmt, args...);
            return std::move(r);
        }
    };

#define __WHEN__POSTFIX return ::nm::__WHEN__OBJECT__::get()
#define WHEN(condition) \
    if(condition) __WHEN__POSTFIX
#define WHEN_NUL_1(v1) \
    if(nul(v1)) __WHEN__POSTFIX
#define WHEN_NUL_2(v1, v2) \
    if(nul(v1) || nul(v2)) __WHEN__POSTFIX
#define WHEN_NUL_3(v1, v2, v3) \
    if(nul(v1) || nul(v2) || nul(v3)) __WHEN__POSTFIX
#define WHEN_NUL_4(v1, v2, v3, v4) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4)) __WHEN__POSTFIX
#define WHEN_NUL_5(v1, v2, v3, v4, v5) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5)) __WHEN__POSTFIX
#define WHEN_NUL_6(v1, v2, v3, v4, v5, v6) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5) || nul(v6)) __WHEN__POSTFIX
#define WHEN_NUL(...) NM_OVERLOAD(WHEN_NUL, __VA_ARGS__)

} // namespace nm
