#include "core/worker/supply/srcSupply.hpp"

#include "core/ast/node.hpp"
#include "core/worker/bison/lowscanner.hpp"
#include "core/builtin/err/nerr.hpp"

namespace by {
    BY_DEF_ME(srcSupply)

    void* me::_scanString(parser& ps, const nchar* src, void* scanner) const {
        if(!src || src[0] == '\0') return ps.getReport().add(nerr::newErr(errCode::SRC_EMPTY)), nullptr;

        return yy_scan_string((nchar*) src, (yyscan_t) scanner);
    }

    exprMaker& me::_getMaker(parser& ps) const { return ps._getMaker(); }
}
