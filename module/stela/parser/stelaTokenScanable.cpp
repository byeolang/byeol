#include "stela/parser/stelaTokenScanable.hpp"

namespace by {

    BY(DEF_ME(stelaTokenScanable))

    nint me::onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner) {
        nbool dum;
        return onScan(ps, val, loc, scanner, dum);
    }
}
