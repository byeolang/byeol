/**
 * @file
 * Basic type definitions used across the byeol language system
 */
#pragma once

#include "indep/def/common.hpp"
#include "indep/common/dep.hpp"

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
    constexpr nidx BY_INDEX_ERROR = -1;
} // namespace by
