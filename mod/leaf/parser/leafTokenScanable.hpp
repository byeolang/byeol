#pragma once

#include "../common.hpp"

typedef union ZZSTYPE ZZSTYPE;
typedef struct lloc ZZLTYPE;
typedef void* zzscan_t;

namespace nm {

    class leafParser;

    class _nout leafTokenScanable {
        NM(ME(leafTokenScanable))

    public:
        nint onScan(leafParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        virtual nint onScan(leafParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner,
            nbool& isBypass) = 0;
    };
}
