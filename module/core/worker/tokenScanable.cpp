#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/container/native/tnmap.inl"
#include "core/ast/node.hpp"
#include "core/worker/tokenScanable.hpp"

namespace by {

    BY(DEF_ME(tokenScanable))

    nint me::onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner) {
        nbool dum;
        return onScan(ps, val, loc, scanner, dum);
    }
}
