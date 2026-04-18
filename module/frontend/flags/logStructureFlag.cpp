#include "frontend/flags/logStructureFlag.hpp"
#include "frontend/cli.hpp"

namespace by {

    BY_DEF_ME(logStructureFlag)

    me::logStructureFlag(cli& c): _cli(c) {}

    const nchar* me::getName() const { return "-S, --show-structure"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    this flag prints all log about structure of parsed ast.
    by inspecting the structure, you can figure out that those asts are parsed properly.
    it also shows frame info which contains the stackframe when latest error occurs.)DESC";
    }

    const strings& me::_getRegExpr() const {
        static strings inner{"^\\-S", "^\\--show-structure$"};
        return inner;
    }

    me::res me::_onTake(const flagArgs&) const {
        _cli.getInterpreter().addFlag(interpreter::LOG_STRUCTURE);
        _cli.getStarter().addFlag(starter::LOG_STRUCTURE);
        return MATCH;
    }
}
