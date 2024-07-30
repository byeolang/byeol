#include "fileSupply.hpp"

#include "../parser.hpp"

namespace nm {
    NM(DEF_ME(fileSupply))

    me::fileSupply(const std::string& newPath): _path(newPath) {}

    void* me::onSupplySrc(parser& ps, void* scanner) const {
        static std::string dummy;
        std::ifstream fout(_path);
        if(fout.fail())
            // there is no file.
            return ps.getReport().add(err::newErr(errCode::FILE_NOT_OPEN, _path.c_str())), nullptr;

        std::stringstream buf;
        buf << fout.rdbuf();
        std::string codes = buf.str();
        void* ret = _scanString(ps, codes.c_str(), scanner);
        if(ret) _getMaker(ps).setSrcFile(*new srcFile(_path, codes));
        return ret;
    }
}
