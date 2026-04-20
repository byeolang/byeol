/**
 * @file
 * Basic type definitions used across the byeol language system
 */
#pragma once

#include <cstdint>

#ifdef WIN32
#    undef _nout
#    define _nhidden
#    ifdef __flagStacker = 1 // I'm currently building flagStacker module
#        define _nout __declspec(dllexport)
#    else
#        define _nout __declspec(dllimport)
#    endif
#else
#    undef _nout
#    define _nout __attribute__((visibility("default")))
#    define _nhidden __attribute__((visibility("hidden")))
#endif

#ifdef UNICODE
#    define tchar wchar_t
#    define tmain wmain
#    define tcslen wcslen
#    define tcscat wcscat
#    define tcscpy wcscpy
#    define tcsncpy wcsncpy
#    define tcscmp wcscmp
#    define tcsncmp wcsncmp
#    define tprintf wprintf
#    define tscanf wscanf
#    define fgetts fgetws
#    define fputts fputws
#else
#    define tchar char
#    define tcslen strlen
#    define tcscat strcat
#    define tcscpy strcpy
#    define tcsncpy strncpy
#    define tcscmp strcmp
#    define tcsncmp strncmp
#    define tprintf printf
#    define tscanf scanf
#    define fgetts fgets
#    define fputts fputs
#endif

namespace by {
    typedef int nint;
    typedef unsigned int nuint;
    typedef bool nbool;
    typedef char nchar;
    typedef unsigned char nuchar;
    typedef wchar_t nwchar;
    typedef tchar ntchar;
    typedef short int nshort;
    typedef unsigned short int nushort;
    typedef float nflt;
    typedef double ndbl;
    typedef long nlong;
    typedef unsigned long nulong;
    typedef long long nllong;
    typedef unsigned long long nullong;
    typedef std::int8_t nint8;
    typedef std::uint8_t nuint8;
    typedef std::int16_t nint16;
    typedef std::uint16_t nuint16;
    typedef std::int32_t nint32;
    typedef std::uint32_t nuint32;
    typedef std::int64_t nint64;
    typedef std::uint64_t nuint64;

    typedef nint nidx;
    typedef nchar nbyte;
    typedef nuchar nubyte;
    typedef nint64 nid;
    typedef nint ncnt;
} // namespace by
