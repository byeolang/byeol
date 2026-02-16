#include "core/worker/supply/bufSupply.hpp"

#include "core/ast/exprs/exprMaker.hpp"
#include "core/ast/node.hpp"
#include "core/common/coreInternal.hpp"

namespace by {
    BY_DEF_ME(bufSupply)

    me::bufSupply(const std::string& buf): _buf(buf) {}

    void* me::onSupplySrc(parser& ps, void* scanner) const {
        coreInternal::getMaker(ps).setSrcFile(*new srcFile("no_file", _buf));
        return _scanString(ps, _buf.c_str(), scanner);
    }
}
