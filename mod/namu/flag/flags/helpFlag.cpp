#include "helpFlag.hpp"

#include "../../cli.hpp"

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
        _printUsage();

        const flags& opts = c.getFlags();
        for(int n = 0; n < opts.size(); n++) {
            const flag& opt = opts[n].get();
            std::cout << "   " << opt.getName() << opt.getDescription() << "\n\n";
        }

        return EXIT_PROGRAM; // don't want to keep processing. just exit program.
    }

    void helpFlag::_printUsage() const {
        std::cout << "Namu interpreter. " << buildFeature::config::getName() << " binary built on "
                  << buildFeature::date::getMonth() << "-" << buildFeature::date::getYear() << ". v"
                  << buildFeature::version::getValue() << "\n";
        std::cout << "Usage: namu <option> <filepath>\n";
    }
}
