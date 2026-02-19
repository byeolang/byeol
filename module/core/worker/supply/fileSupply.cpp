#include "core/worker/supply/fileSupply.hpp"

#include "core/worker/parser.hpp"
#include "core/internal/coreInternal.hpp"

namespace by {
    BY(DEF_ME(fileSupply))

    me::fileSupply(const std::string& newPath): _path(newPath) {}

    void* me::onSupplySrc(parser& ps, void* scanner) const {
        static std::string dummy;
        std::ifstream fout(_path);
        if(fout.fail()) { // there is no file.
            ps.getReport().add(nerr::newErr(errCode::FILE_NOT_OPEN, _path.c_str()));
            return nullptr;
        }

        std::stringstream buf;
        buf << fout.rdbuf();
        std::string codes = buf.str();
        void* ret = _scanString(ps, codes.c_str(), scanner);
        if(ret) coreInternal::getMaker(ps).setSrcFile(*new srcFile(_path, codes));
        return ret;
    }
}
