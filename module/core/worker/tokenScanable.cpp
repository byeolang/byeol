#include "core/ast/node.hpp"
#include "core/worker/tokenScanable.hpp"

namespace by {

    NM(DEF_ME(tokenScanable))

    nint me::onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
        nbool dum;
        return onScan(ps, val, loc, scanner, dum);
    }
}
