#pragma once

#include "dep.hpp"

namespace nm {

    class errReport;
    class node;

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __core_when__

    class _nout __WHEN_OBJECT__: public __clog_when__ {
        NM(ME(__WHEN_OBJECT__, __clog_when__))

    public:
        static const me& get();

        template <typename... Ts> const me& exErr(int code, const Ts&... args) const {
            _addNewErr(code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(const point& src, int code, const Ts&... args) const {
            _addNewErr(src, code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(const node& src, int code, const Ts&... args) const {
            _addNewErr(_getPosFrom(src), code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(const point& src, int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(src, code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exErr(const node& src, int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(_getPosFrom(src), code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

    private:
        errReport& _getDefault() const;
        const point& _getPosFrom(const node& src) const;
        void _addNewErr(int code, errReport* rpt, ...) const;
        void _addNewErr(const point& src, int code, errReport* rpt, ...) const;
    };
} // namespace nm
