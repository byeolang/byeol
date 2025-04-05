#pragma once

#include "dep.hpp"

namespace nm {

    class errReport;

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __core_when__

    class _nout __WHEN_OBJECT__ : public __clog_when__ {
        NM(ME(__WHEN_OBJECT__, __clog_when__))

    public:
        static const me& get();

        void exErr(int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(_getDefault(), code, args);
            va_end(args);
        }

        void exErr(const point& src, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(_getDefault(), src, code, args);
            va_end(args);
        }

        template <typename R>
        R& exErr([[maybe_unused]] R& r, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(_getDefault(), code, args);
            va_end(args);
            return r;
        }

        template <typename R>
        R& exErr([[maybe_unused]] R& r, const point& src, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(_getDefault(), src, code, args);
            va_end(args);
            return r;
        }

        template <typename R>
        R&& exErr([[maybe_unused]] R&& r, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(_getDefault(), code, args);
            va_end(args);
            return std::move(r);
        }

        template <typename R>
        R&& exErr([[maybe_unused]] R&& r, const point& src, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(_getDefault(), src, code, args);
            va_end(args);
            return std::move(r);
        }

        void exErr(errReport& rpt, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(rpt, code, args);
            va_end(args);
        }

        void exErr(errReport& rpt, const point& src, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(rpt, src, code, args);
            va_end(args);
        }

        template <typename R>
        R& exErr([[maybe_unused]] R& r, errReport& rpt, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(rpt, code, args);
            return r;
        }

        template <typename R>
        R& exErr([[maybe_unused]] R& r, errReport& rpt, const point& src, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(rpt, src, code, args);
            return r;
        }

        template <typename R>
        R&& exErr([[maybe_unused]] R&& r, errReport& rpt, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(rpt, code, args);
            return std::move(r);
        }

        template <typename R>
        R&& exErr([[maybe_unused]] R&& r, errReport& rpt, const point& src, int code, ...) const {
            va_list args;
            va_start(args, code);
            _addNewErr(rpt, src, code, args);
            return std::move(r);
        }

    private:
        errReport& _getDefault() const;
        void _addNewErr(errReport& rpt, int code, va_list args) const;
        void _addNewErr(errReport& rpt, const point& src, int code, va_list args) const;
    };
}
