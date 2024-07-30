#include "logStructureFlag.hpp"

#include "../../cli.hpp"

namespace nm {

    NM_DEF_ME(logStructureFlag)

    const nchar* me::getName() const { return "--show-structure"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    this flag prints all log about structure of parsed ast.
    by inspecting the structure, you can figure out that those asts are parsed properly.
    it also shows frame info which contains the stackframe when latest error occurs.)DESC";
    }

    const nchar* me::_getRegExpr() const { return "^\\--show-structure$"; }

    nbool me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        ip.addFlag(interpreter::LOG_STRUCTURE);
        s.addFlag(starter::LOG_STRUCTURE);
        return true;
    }
}
