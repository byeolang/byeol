#pragma once

#include "verification.hpp"
#include "verifier.hpp"
#include "../../frame/thread.hpp"

namespace wrd {

#define ME verification

    template <typename... Args>
    void ME::_warn(Args... args) {
        _getVerifier().getReport().add(err::newWarn(args...));
        logFrameInfo(thread::get().getNowFrame());
    }
    template <typename... Args>
    void ME::_err(Args... args) {
        _getVerifier().getReport().add(err::newErr(args...));
        logFrameInfo(thread::get().getNowFrame());
    }
    template <typename... Args>
    void ME::_info(Args... args) {
        _getVerifier().getReport().add(err::newInfo(args...));
    }

    template <typename... Args>
    void ME::_srcWarn(Args... args) {
        _getVerifier().getReport().add(err::newWarn(_getIt().getPos(), args...));
        logFrameInfo(thread::get().getNowFrame());
    }
    template <typename... Args>
    void ME::_srcErr(Args... args) {
        _getVerifier().getReport().add(err::newErr(_getIt().getPos(), args...));
        logFrameInfo(thread::get().getNowFrame());
    }
    template <typename... Args>
    void ME::_srcInfo(Args... args) {
        _getVerifier().getReport().add(err::newInfo(_getIt().getPos(), args...));
    }

#undef ME
}
