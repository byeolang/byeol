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
}

/**
해야할일:
    B. 지금의 `1개의 파일당 1개의 srcSupply 정책`을 유지한다. 이 경우 dirFlag에서 직접 fsystem으로 파일을 탐색해서
       각 .byeol 파일에 대해 srcSupply 객체를 추가한다.
 */
