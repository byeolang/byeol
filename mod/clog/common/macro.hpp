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
//          NM_I("just message.")
//          NM_I("with format=%x string=%s", &format, format.c_str())
#define NM_E(fmt, ...) ::nm::richLog(nm::logLv::ERR, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NM_W(fmt, ...) ::nm::richLog(nm::logLv::WARN, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#define NM_I(fmt, ...) ::nm::richLog(nm::logLv::INFO, __FILENAME__, __func__, __LINE__, fmt "\n", ## __VA_ARGS__)
#ifdef NM_DEBUG
#   define NM_DE(fmt, ...) NM_E(fmt, ## __VA_ARGS__)
#   define NM_DW(fmt, ...) NM_W(fmt, ## __VA_ARGS__)
#   define NM_DI(fmt, ...) NM_I(fmt, ## __VA_ARGS__)
#else
#   define NM_DE(fmt, ...) void()
#   define NM_DW(fmt, ...) void()
#   define NM_DI(fmt, ...) void()
#endif

#define NM_RET_1(condition, macro) NM_RET_3(condition, macro, #condition, void())
#define NM_RET_2(condition, macro, retVal) NM_RET_3(condition, macro, #condition, retVal)
#define NM_RET_3(condition, macro, msg, retVal) if(condition) return macro(msg), retVal
#define NM_RET(...) NM_OVERLOAD(NM_RET, __VA_ARGS__)

#define NM_ERR_IF_1(condition) NM_RET_1(condition, NM_E)
#define NM_ERR_IF_2(condition, retVal) NM_RET_2(condition, NM_E, retVal)
#define NM_ERR_IF_3(condition, msg, retVal) NM_RET_3(condition, NM_E, msg, retVal)
#define NM_ERR_IF(...) NM_OVERLOAD(NM_ERR_IF, __VA_ARGS__)

#define NM_WARN_IF_1(condition) NM_RET_1(condition, NM_W)
#define NM_WARN_IF_2(condition, retVal) NM_RET_2(condition, NM_W, retVal)
#define NM_WARN_IF_3(condition, msg, retVal) NM_RET_3(condition, NM_W, msg, retVal)
#define NM_WARN_IF(...) NM_OVERLOAD(NM_WARN_IF, __VA_ARGS__)

#define NM_INFO_IF_1(condition) NM_RET_1(condition, NM_I)
#define NM_INFO_IF_2(condition, retVal) NM_RET_2(condition, NM_I, retVal)
#define NM_INFO_IF_3(condition, msg, retVal) NM_RET_3(condition, NM_I, msg, retVal)
#define NM_INFO_IF(...) NM_OVERLOAD(NM_INFO_IF, __VA_ARGS__)
