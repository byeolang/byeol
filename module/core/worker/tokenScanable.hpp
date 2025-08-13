#pragma once

#include "core/type/ntype.hpp"

typedef union YYSTYPE YYSTYPE;
typedef struct lloc YYLTYPE;
typedef void* yyscan_t;

namespace by {

    class parser;

    class _nout tokenScanable {
        BY(ME(tokenScanable))

    public:
        nint onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner);
        virtual nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner, nbool& isBypass) = 0;
    };
}
