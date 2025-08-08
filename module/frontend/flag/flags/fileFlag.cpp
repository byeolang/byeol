#include "frontend/flag/flags/fileFlag.hpp"

#include <fstream>
#include <sstream>

namespace by {
    NM_DEF_ME(fileFlag)

    const nchar* me::getName() const { return "<filePath>"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    file path of frontend language script file.
    extension of file should be end with 'nm'.
    wildcard and the question mark aren't allowed.)DESC";
    }

    const strings& me::_getRegExpr() const {
        static strings inner{"[\\\\\\w\\.\\/:\\-\\(\\)\\d]+\\.nm$"};
        return inner;
    }

    me::res me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        for(const auto& filePath: tray)
            ip.addSupply(*new fileSupply(filePath));
        return MATCH;
    };
}
