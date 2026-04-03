#include "frontend/flag/flags/fileFlag.hpp"

#include <fstream>
#include <sstream>

namespace by {
    BY_DEF_ME(fileFlag)

    const nchar* me::getName() const { return "<file path>"; }

    const nchar* me::getDescription() const {
        static std::string inner = std::string(R"DESC(
    file path of frontend language script file.
    extension of file should be end with ')DESC") + FILE_EXTENSION + R"DESC('.
    wildcard and the question mark aren't allowed.)DESC";
        return inner.c_str();
    }

    const strings& me::_getRegExpr() const {
        static strings inner{
            std::string("[^\\@\\#\\$\\%\\[\\]\\+\\\\\\|\\<\\>\\=\\!\\^\\&\\*\\(\\)\\{\\}\\n\\s]+\\.") + FILE_EXTENSION
        };
        return inner;
    }

    me::res me::_onTake(const flagArgs& tray, cli&, interpreter& ip, starter&, errReport&) const {
        for(const auto& filePath: tray)
            ip.getParser().addSupply(*new fileSupply(filePath.get()));
        return MATCH;
    };
}
