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
        std::cout << "🌟 'byeol' " << buildFeature::version::get() << "\n"
                  << "Built on " << buildFeature::date::get() << " (build " << buildFeature::version::getBuildCount()
                  << ", git " << buildFeature::version::getCommitHash() << ", " << buildFeature::config::getName()
                  << ", " << buildFeature::platform::getArchBits() << "-bit)\n"
                  << "© 2010-" << buildFeature::date::getYear()
                  << " Taehun Lee(kniz) and contributors. All rights reserved.\n";

        return EXIT_PROGRAM; // don't want to keep processing. just exit program.
    }
}
