#include "frontend/flag/flags/fileFlag.hpp"

#include <fstream>
#include <sstream>

namespace by {
    BY_DEF_ME(fileFlag)

#define FILE_EXTENSION "byeol"

    const nchar* me::getName() const { return "<filePath>"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    file path of frontend language script file.
    extension of file should be end with ')DESC" FILE_EXTENSION R"DESC('.
    wildcard and the question mark aren't allowed.)DESC";
    }

    const strings& me::_getRegExpr() const {
        static strings inner{"[^\\@\\#\\$\\%\\[\\]\\+\\\\\\|\\<\\>\\=\\!\\^\\&\\*\\(\\)\\{\\}\\n\\s]+\\." FILE_EXTENSION};
        return inner;
    }

    me::res me::_onTake(const flagArgs& tray, cli&, interpreter& ip, starter&, errReport&) const {
        for(const auto& filePath: tray)
            ip.getParser().addSupply(*new fileSupply(filePath));
        return MATCH;
    };
}
