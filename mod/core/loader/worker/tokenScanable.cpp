#include "tokenScanable.hpp"

namespace nm {

    NAMU(DEF_ME(tokenScanable))

    nint me::onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
        nbool dum;
        return onScan(ps, val, loc, scanner, dum);
    }
}
