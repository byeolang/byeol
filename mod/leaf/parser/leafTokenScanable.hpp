#pragma once

#include "../common.hpp"

typedef union YYSTYPE YYSTYPE;
typedef struct lloc YYLTYPE;
typedef void* yyscan_t;

namespace nm {

    class leafParser;

    class _nout leafTokenScanable {
        NM(ME(leafTokenScanable))

    public:
        nint onScan(leafParser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner);
        virtual nint onScan(leafParser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner,
            nbool& isBypass) = 0;
    };
}
