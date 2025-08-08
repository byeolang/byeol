#include "frontend/flag/flags/verFlag.hpp"

#include "frontend/cli.hpp"

namespace nm {

    NM(DEF_ME(verFlag))

    const nchar* verFlag::getName() const { return "--version"; }

    const nchar* verFlag::getDescription() const {
        return R"DESC(
    show current version of frontend cli client.)DESC";
    }

    const strings& verFlag::_getRegExpr() const {
        static strings inner{"^\\--version$"};
        return inner;
    }

    me::res verFlag::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        std::cout << "frontend interpreter. " << buildFeature::config::getName() << " binary built on "
                  << buildFeature::date::getMonth() << "-" << buildFeature::date::getYear() << ". v"
                  << buildFeature::version::getValue() << "\n";
        return EXIT_PROGRAM; // don't want to keep processing. just exit program.
    }
}
