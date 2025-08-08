#include "leaf/parser/leafTokenScanable.hpp"

namespace by {

    NM(DEF_ME(leafTokenScanable))

    nint me::onScan(leafParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner) {
        nbool dum;
        return onScan(ps, val, loc, scanner, dum);
    }
}
