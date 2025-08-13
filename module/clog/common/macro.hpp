#pragma once

//  Log macro:
//      prints debug log on console and file.
//      you can check the file recorded logs at your current work directory
//      named to "logs".
//
//      if you dbg version macros, _DI, _DE, _DW, then log msg will be left
//      only if you use dbg configuration.
//
//      usage:
//          BY_I("just message.")
//          BY_I("with format=%x string=%s", &format, format.c_str())
#ifndef __EMSCRIPTEN__
#    define BY_E(fmt, ...) ::by::richLog(::by::errLv::ERR, __FILENAME__, __func__, __LINE__, fmt "\n", ##__VA_ARGS__)
#    define BY_W(fmt, ...) ::by::richLog(::by::errLv::WARN, __FILENAME__, __func__, __LINE__, fmt "\n", ##__VA_ARGS__)
#    define BY_I(fmt, ...) ::by::richLog(::by::errLv::INFO, __FILENAME__, __func__, __LINE__, fmt "\n", ##__VA_ARGS__)
#else
#    define BY_E(fmt, ...) void()
#    define BY_W(fmt, ...) void()
#    define BY_I(fmt, ...) void()
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
