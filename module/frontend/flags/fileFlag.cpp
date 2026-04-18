#include "frontend/flags/fileFlag.hpp"
#include "frontend/cli.hpp"

#include <fstream>
#include <sstream>

namespace by {

    BY_DEF_ME(fileFlag)

    me::fileFlag(cli& c): _cli(c) {}

    const nchar* me::getName() const { return "<file path>"; }

    const nchar* me::getDescription() const {
        static std::string inner = std::string(R"DESC(
    file path of frontend language script file.
    extension of file should be end with ')DESC") +
            util::FILE_EXTENSION + R"DESC('.
    wildcard and the question mark aren't allowed.)DESC";
        return inner.c_str();
    }

    const strings& me::_getRegExpr() const {
        static strings inner{util::FILE_REGEX};
        return inner;
    }

    me::res me::_onTake(const flagArgs& tray) const {
        for(const auto& filePath: tray)
            _cli.getInterpreter().getParser().addSupply(*new fileSupply(filePath));
        return MATCH;
    };
}
