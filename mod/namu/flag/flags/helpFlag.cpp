#include "helpFlag.hpp"
#include "../../cli.hpp"

namespace nm {

    const nchar* helpFlag::getName() const {
        return "-h";
    }

    const nchar* helpFlag::getDescription() const {
        return R"DESC(
        show help documents.
        list all of available commands and description for how to use them.)DESC";
    }

    const nchar* helpFlag::_getRegExpr() const {
        return "^\\-h$";
    }



    nbool helpFlag::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        _printUsage();

        const flags& opts = c.getFlags();
        for(int n=0; n < opts.size() ;n++) {
            const flag& opt = opts[n].get();
            std::cout   << "   " << opt.getName()
                        << opt.getDescription() << "\n\n";
        }

        return true;
    }

    void helpFlag::_printUsage() const {
        std::cout << "NAMU interpreter by kniz. " << buildFeature::config::getName() << " on " << buildFeature::date::getMonth()
                << "-" << buildFeature::date::getYear() << ". v" << buildFeature::version::getValue() << "\n";
        std::cout << "Usage: <option> <filepath>\n";
    }
}
