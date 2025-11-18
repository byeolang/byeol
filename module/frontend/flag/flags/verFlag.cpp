#include "frontend/flag/flags/verFlag.hpp"

#include "frontend/cli.hpp"

namespace by {

    BY(DEF_ME(verFlag))

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
        constexpr nint year = buildFeature::date::getYear();
        std::cout << "byeol " << buildFeature::version::getValue() << " ("
            << buildFeature::config::getName() << ", "
            << buildFeature::platform::getArchBits() << "-bit)\n"
            << "Built on " << buildFeature::date::getMonth() << " " << buildFeature::date::getDay()
            << " " << year << "\n"
            << "© 2010-" << year << " Taehun Lee(kniz) and contributors. All rights reserved.\n";
        return EXIT_PROGRAM; // don't want to keep processing. just exit program.
    }
}
