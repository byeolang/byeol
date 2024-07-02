#include "fileFlag.hpp"
#include <fstream>
#include <sstream>

namespace nm {
    NM_DEF_ME(fileFlag)

    const nchar* me::getName() const {
        return "<filePath>";
    }

    const nchar* me::getDescription() const {
        return R"DESC(
    file path of namu language script file.
    extension of file should be end with 'nm'.
    wildcard and the question mark aren't allowed.)DESC";
    }

    const nchar* me::_getRegExpr() const {
        return "[\\\\\\w\\.\\/:\\-\\(\\)\\d]+\\.nm$";
    }

    nbool me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        for(const auto& filePath : tray)
            ip.addSupply(*new fileSupply(filePath));
        return true;
    };
}
