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

/**
 * @brief Logs scope entry and exit with execution time and depth visualization.
 * @details Uses @ref BY_END and @ref scopeLog for RAII-based scope logging.
 * Graph depth is automatically incremented on entry and decremented on exit.
 * @warning Arguments are evaluated twice. Avoid side-effect expressions.
 * @code
 * void good(int id) {
 *     BY_E_SCOPE("good id=%d", id);  // OK
 *     // ... work ...
 *     BY_I("hello log!")
 * }
 * Outputs:
 *    E <good#130> ▶ good id=123
 *    I <good#131> ┣ hello log!
 *    E <good#130> ◀ good id=123
 *
 * void bad() {
 *     BY_E_SCOPE("count=%d", counter++);  // WRONG: counter increments twice!
 * }
 * @endcode
 *
 *
 * @section be careful when you use BY_?_SCOPE macro in 'if' block statement
 * as you can see below, it defines 'end' instance to catch when the scope ends.
 * so if you use BY_E_SCOPE inside of block statement by expecting it works when the func ends,
 * it won't work as you expected.
 * @code
 *  void foo() {
 *      if(isVerbose) BY_I_SCOPE("okay, verbose mode is on.");
 *      BY_I("hello");
 *  }
 * @endcode
 *
 * in above example, macro will expand like below,
 * @code
 *  void foo() {
 *      if(isVerbose)
 *          ::by::line::incLv();
 *      BY_E("▶  okay, verbose mode is on.");
 *      BY_END(scopeLog, ...., "◀  okay, verbose mode is on.")
 *      BY_I("hello");
 *  }
 * @endcode
 *
 * so the start and end of scope logs always printed.
 */
#    define BY_E_SCOPE(fmt, ...)        \
        ::by::line::incLv();            \
        BY_E("▶  " fmt, ##__VA_ARGS__); \
        BY_END(scopeLog, [&](nllong elapsed) { BY_E("◀  " fmt, ##__VA_ARGS__); });
#    define BY_W_SCOPE(fmt, ...)        \
        ::by::line::incLv();            \
        BY_W("▶  " fmt, ##__VA_ARGS__); \
        BY_END(scopeLog, [&](nllong elapsed) { BY_W("◀  " fmt, ##__VA_ARGS__); });
#    define BY_I_SCOPE(fmt, ...)        \
        BY_I("▶  " fmt, ##__VA_ARGS__); \
        ::by::line::incLv();            \
        BY_END(scopeLog, [&](nllong elapsed) { BY_I("◀  " fmt, ##__VA_ARGS__); });
#else
#    define BY_E(fmt, ...) void()
#    define BY_W(fmt, ...) void()
#    define BY_I(fmt, ...) void()
#    define BY_E_SCOPE(fmt, ...) void()
#    define BY_W_SCOPE(fmt, ...) void()
#    define BY_I_SCOPE(fmt, ...) void()
#endif
#ifdef BY_DEBUG
#    define BY_DE(fmt, ...) BY_E(fmt, ##__VA_ARGS__)
#    define BY_DW(fmt, ...) BY_W(fmt, ##__VA_ARGS__)
#    define BY_DI(fmt, ...) BY_I(fmt, ##__VA_ARGS__)
#    define BY_DE_SCOPE(fmt, ...)        \
        ::by::line::incLv();             \
        BY_DE("▶  " fmt, ##__VA_ARGS__); \
        BY_END(scopeLog, [&](nllong elapsed) { BY_DE("◀  " fmt, ##__VA_ARGS__); });
#    define BY_DW_SCOPE(fmt, ...)        \
        ::by::line::incLv();             \
        BY_DW("▶  " fmt, ##__VA_ARGS__); \
        BY_END(scopeLog, [&](nllong elapsed) { BY_DW("◀  " fmt, ##__VA_ARGS__); });
#    define BY_DI_SCOPE(fmt, ...)        \
        ::by::line::incLv();             \
        BY_DI("▶  " fmt, ##__VA_ARGS__); \
        BY_END(scopeLog, [&](nllong elapsed) { BY_DI("◀  " fmt, ##__VA_ARGS__); });
#else
#    define BY_DE(fmt, ...) void()
#    define BY_DW(fmt, ...) void()
#    define BY_DI(fmt, ...) void()
#    define BY_DE_SCOPE(fmt, ...) void()
#    define BY_DW_SCOPE(fmt, ...) void()
#    define BY_DI_SCOPE(fmt, ...) void()
#endif

#undef BY_SIDE_FUNC_3
#define BY_SIDE_FUNC_3(paramPtr, expr, ret) \
    { return paramPtr ? expr : ret; }
