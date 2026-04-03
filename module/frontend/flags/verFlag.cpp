#include "frontend/flags/verFlag.hpp"
#include "frontend/cli.hpp"

namespace by {

    BY(DEF_ME(verFlag))

    const nchar* me::getName() const { return "--version"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    show current version of frontend cli client.)DESC";
    }

    const strings& me::_getRegExpr() const {
        static strings inner{"^\\--version$"};
        return inner;
    }

    me::res me::_onTake(const flagArgs&) const {
        std::cout << "🌟 'byeol' " << buildFeature::version::get() << "\n"
                  << "Built on " << buildFeature::date::get() << " (git " << buildFeature::version::getCommitHash()
                  << ", " << buildFeature::config::getName()
                  << ", " << buildFeature::platform::getArchBits() << "-bit)\n"
                  << "© 2010-" << buildFeature::date::getYear()
                  << " Taehun Lee(kniz) and contributors. All rights reserved.\n";

        return EXIT_PROGRAM; // don't want to keep processing. just exit program.
    }
}
