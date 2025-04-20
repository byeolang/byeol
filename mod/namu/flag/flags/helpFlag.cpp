#include "namu/flag/flags/helpFlag.hpp"

#include "namu/cli.hpp"

namespace nm {

    NM(DEF_ME(helpFlag))

    const nchar* helpFlag::getName() const { return "-h, --help"; }

    const nchar* helpFlag::getDescription() const {
        return R"DESC(
    show help documents.
    list all of available commands and description for how to use them.)DESC";
    }

    const strings& helpFlag::_getRegExpr() const {
        static strings inner{"^\\-h$", "^\\--help$"};
        return inner;
    }

    me::res helpFlag::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        std::cout << "Usage: namu <option> <filepath>\n";

        const flags& opts = c.getFlags();
        for(int n = 0; n < opts.size(); n++) {
            const flag& opt = opts[n].get();
            std::cout << "   " << opt.getName() << opt.getDescription() << "\n\n";
        }

        return EXIT_PROGRAM; // don't want to keep processing. just exit program.
    }
}
