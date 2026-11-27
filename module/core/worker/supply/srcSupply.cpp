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
        WHEN(_isSrcFile(path)) .ret(tnarr<srcSupply>{*new fileSupply(path)});

        tnarr<srcSupply> ret;
        auto e = fsystem::find(path);
        while(e.next()) {
            // path::operator string_type() gives a wstring on windows, so the
            //  narrow form has to be asked for explicitly.
            std::string filePath = *e;
            if(!_isSrcFile(filePath)) continue;

            ret.add(*new fileSupply(filePath));
        }

        return ret;
    }

    namespace {
        std::string _removeCR(const std::string& src) {
            size_t from = 0, at = src.find("\r\n");
            WHEN(at == std::string::npos).ret(src);

            std::string ret;
            ret.reserve(src.size());
            for(; at != std::string::npos; at = src.find("\r\n", from)) {
                ret.append(src, from, at - from);
                from = at + 1; // keeps the LF.
            }
            ret.append(src, from, std::string::npos);

            return ret;
        }
    }

    void* me::_scanString(parser& ps, const std::string& src, void* scanner) const {
        if(src.empty() || src[0] == '\0') return ps.getReport().add(nerr::newErr(errCode::SRC_EMPTY)), nullptr;

        return yy_scan_string((nchar*) _removeCR(src).c_str(), (yyscan_t) scanner);
    }
} // namespace by
