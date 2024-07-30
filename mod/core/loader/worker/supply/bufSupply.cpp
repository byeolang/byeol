#include "bufSupply.hpp"

#include "../../../ast/exprs/exprMaker.hpp"
#include "../../../ast/node.hpp"

namespace nm {
    NM_DEF_ME(bufSupply)

    me::bufSupply(const std::string& buf): _buf(buf) {}

    void* me::onSupplySrc(parser& ps, void* scanner) const {
        _getMaker(ps).setSrcFile(*new srcFile("no_file", _buf));
        return _scanString(ps, _buf.c_str(), scanner);
    }
}
