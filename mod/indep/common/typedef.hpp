//  ---------------------------------------------------------------------------------
//  파일명  :   commonTypedef.hpp
//  설명    :   노드엔진에서 사용하는 타입 정의
//  메모    :   nidx를 short int로 정의하면, 엔진 전체의 부하가 줄어드는 방식으
//              로 개발되는 것을 목표로 한다.
//  히스토리:   2011-05-29  이태훈  개발 완료
//  ---------------------------------------------------------------------------------
#pragma once

#include "../def/common.hpp"

namespace nm {
    typedef int                 nint;
    typedef unsigned int        nuint;
    typedef bool                nbool;
    typedef char                nchar;
    typedef unsigned char       nuchar;
    typedef wchar_t             nwchar;
    typedef tchar               ntchar;
    typedef short int           nshort;
    typedef unsigned short int  nushort;
    typedef float               nflt;
    typedef double              ndbl;
    typedef long                nlong;
    typedef unsigned long       nulong;
    typedef int64_t             nint64;
    typedef uint64_t            nuint64;

    typedef nint                nidx;
    typedef nchar               nbyte;
    typedef nuchar              nubyte;
    typedef nint64              nid;
    typedef nint                ncnt;
    constexpr nidx NM_INDEX_ERROR = -1;
}
