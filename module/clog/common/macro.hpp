/** @file
 *  Log macro definitions for byeol language logging system
 */
#pragma once

#include "clog/common/dep.hpp"

/**
 *  @brief Log macro: prints debug log on console and file.
 *  @details You can check the file recorded logs at your current work directory
 *  named to "logs".
 *
 *  If you dbg version macros, _DI, _DE, _DW, then log msg will be left
 *  only if you use dbg configuration.
 *
 *  @code
 *  BY_I("just message.")
 *  BY_I("with format=%x string=%s", &format, format.c_str())
 *  @endcode
 */
#ifndef __EMSCRIPTEN__
#    define BY_E(fmt, ...) ::by::richLog(::by::errLv::ERR, __FILENAME__, __func__, __LINE__, fmt "\n", ##__VA_ARGS__)
#    define BY_W(fmt, ...) ::by::richLog(::by::errLv::WARN, __FILENAME__, __func__, __LINE__, fmt "\n", ##__VA_ARGS__)
#    define BY_I(fmt, ...) ::by::richLog(::by::errLv::INFO, __FILENAME__, __func__, __LINE__, fmt "\n", ##__VA_ARGS__)

/// @brief Logs scope entry and exit automatically using RAII pattern.
/// @details Uses @ref BY_END to defer exit log until scope end.
/// @warning Arguments are evaluated twice. Avoid side-effect expressions.
/// @code
/// void good(int id) {
///     BY_E_SCOPE("good id=%d", id);  // OK
///     BY_I("doSomething");
///     // ... work ...
/// }
/// // Outputs: 
///     Dec 25 2025  12:22:06 E generaliz <good#130> [START] --> good id=123"
///     Dec 25 2025  12:22:06 I generaliz <good#131> doSomething
///     Dec 25 2025  12:22:06 E generaliz <good#132> [END] <-- process id=123
///
/// void bad() {
///     BY_E_SCOPE("count=%d", counter++);  // WRONG: counter increments twice!
/// }
/// @endcode
#    define BY_E_SCOPE(fmt, ...) \
        BY_E(">>> START: " fmt, ## __VA__ARGS__); \
        BY_END(BY_E("<<< END: " fmt, ## __VA_ARGS__))
#else
#    define BY_E(fmt, ...) void()
#    define BY_W(fmt, ...) void()
#    define BY_I(fmt, ...) void()
#    define BY_E_SCOPE(fmt, ...) void()
#endif
#ifdef BY_DEBUG
#    define BY_DE(fmt, ...) BY_E(fmt, ##__VA_ARGS__)
#    define BY_DW(fmt, ...) BY_W(fmt, ##__VA_ARGS__)
#    define BY_DI(fmt, ...) BY_I(fmt, ##__VA_ARGS__)
#else
#    define BY_DE(fmt, ...) void()
#    define BY_DW(fmt, ...) void()
#    define BY_DI(fmt, ...) void()
#endif

#undef BY_SIDE_FUNC_3
#define BY_SIDE_FUNC_3(paramPtr, expr, ret) \
    { return paramPtr ? expr : ret; }
