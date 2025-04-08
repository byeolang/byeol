#pragma once

#include "dep.hpp"

namespace nm {

    class errReport;

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __core_when__

    class _nout __WHEN_OBJECT__: public __clog_when__ {
        NM(ME(__WHEN_OBJECT__, __clog_when__))

    public:
        static const me& get();

        template <typename... Ts> const me& exErr(int code, const Ts&... args) const {
            _addNewErr(_getDefault(), code, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(const point& src, int code, const Ts&... args) const {
            _addNewErr(_getDefault(), src, code, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(rpt, code, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(const point& src, int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(rpt, src, code, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

    private:
        errReport& _getDefault() const;
        void _addNewErr(errReport& rpt, int code, ...) const;
        void _addNewErr(errReport& rpt, const point& src, int code, ...) const;
    };
} // namespace nm
