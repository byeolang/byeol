#include "leafTokenScanable.hpp"

namespace nm {

    NM(DEF_ME(leafTokenScanable))

    nint me::onScan(leafParser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
        nbool dum;
        return onScan(ps, val, loc, scanner, dum);
    }
}
