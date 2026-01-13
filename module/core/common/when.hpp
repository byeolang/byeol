/// @file
#pragma once

#include "core/common/dep.hpp"

namespace by {

    class errReport;
    class node;

#undef __WHEN_OBJECT__
#define __WHEN_OBJECT__ __core_when__

    /** @ingroup core
     *  @brief Enhanced error handling for core module
     *  @details Extended WHEN macro support with node-aware error reporting.
     */
    class _nout __WHEN_OBJECT__: public __clog_when__ {
        BY(ME(__WHEN_OBJECT__, __clog_when__))

    public:
        static const me& get();

        template <typename... Ts> const me& exErr(int code, const Ts&... args) const {
            _addNewErr(errLv::ERR, code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exErr(const point& src, int code, const Ts&... args) const {
            _addNewErr(errLv::ERR, src, code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exErr(const node& src, int code, const Ts&... args) const {
            _addNewErr(errLv::ERR, _getPosFrom(src), code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exErr(int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(errLv::ERR, code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exErr(const point& src, int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(errLv::ERR, src, code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exErr(const node& src, int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(errLv::ERR, _getPosFrom(src), code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exWarn(int code, const Ts&... args) const {
            _addNewErr(errLv::ERR, code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exWarn(const point& src, int code, const Ts&... args) const {
            _addNewErr(errLv::WARN, src, code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exWarn(const node& src, int code, const Ts&... args) const {
            _addNewErr(errLv::WARN, _getPosFrom(src), code, &_getDefault(), __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exWarn(int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(errLv::WARN, code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts>
        const me& exWarn(const point& src, int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(errLv::WARN, src, code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

        template <typename... Ts> const me& exWarn(const node& src, int code, errReport& rpt, const Ts&... args) const {
            _addNewErr(errLv::WARN, _getPosFrom(src), code, &rpt, __convert__((const Ts&) args).unwrap()...);
            return *this;
        }

    private:
        errReport& _getDefault() const;
        const point& _getPosFrom(const node& src) const;
        const point& _getPosFrom(const node* src) const;
        void _addNewErr(errLv::level lv, int code, errReport* rpt, ...) const;
        void _addNewErr(errLv::level lv, const point& src, int code, errReport* rpt, ...) const;
    };
} // namespace by
