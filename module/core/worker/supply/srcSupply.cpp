#include "core/worker/supply/srcSupply.hpp"

#include "core/ast/node.hpp"
#include "core/worker/bison/lowscanner.hpp"
#include "core/builtin/err/nerr.hpp"
#include "core/worker/supply/fileSupply.hpp"

namespace by {
    BY_DEF_ME(srcSupply)

    namespace {
        static nbool _isSrcFile(const std::string& path) {
            std::regex re(util::FILE_REGEX);
            return std::regex_search(path, re);
        }
    }

    srcSupplies me::makeSuppliesFrom(const std::string& path) {
        WHEN(_isSrcFile(path)).ret(tnarr<srcSupply>{*new fileSupply(path)});

        tnarr<srcSupply> ret;
        auto e = fsystem::find(path);
        while(e.next()) {
            const std::string& filePath = *e;
            if(!_isSrcFile(filePath)) continue;

            ret.add(*new fileSupply(filePath));
        }

        return ret;
    }

    void* me::_scanString(parser& ps, const nchar* src, void* scanner) const {
        if(!src || src[0] == '\0') return ps.getReport().add(nerr::newErr(errCode::SRC_EMPTY)), nullptr;

        return yy_scan_string((nchar*) src, (yyscan_t) scanner);
    }
}
