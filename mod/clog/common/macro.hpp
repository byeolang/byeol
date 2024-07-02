#pragma once

#define _LOG(func, level, fmt, ...) {                                         \
}

//  Log macro:
//      prints debug log on console and file.
//      you can check the file recorded logs at your current work directory
//      named to "logs".
//
//      if you dbg version macros, _DI, _DE, _DW, then log msg will be left
//      only if you use dbg configuration.
//
//      usage:
//          NAMU_I("just message.")
//          NAMU_I("with format=%x string=%s", &format, format.c_str())
#define NAMU_E(fmt, ...) ::nm::logger::get().log(nm::logLv::ERR, NAMU_TAG, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NAMU_W(fmt, ...) ::nm::logger::get().log(nm::logLv::WARN, NAMU_TAG, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NAMU_I(fmt, ...) ::nm::logger::get().log(nm::logLv::INFO, NAMU_TAG, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NAMU_EX(code, ...) this->ex(code, __VA_ARGS__)
#ifdef NAMU_DEBUG
#   define NAMU_DE(fmt, ...) NAMU_E(fmt, ## __VA_ARGS__)
#   define NAMU_DW(fmt, ...) NAMU_W(fmt, ## __VA_ARGS__)
#   define NAMU_DI(fmt, ...) NAMU_I(fmt, ## __VA_ARGS__)
#else
#   define NAMU_DE(fmt, ...) void()
#   define NAMU_DW(fmt, ...) void()
#   define NAMU_DI(fmt, ...) void()
#endif
