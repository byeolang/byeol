#include "indep/helper/errorable.hpp"
#include "indep/helper/buildFeature.hpp"

namespace by {
    BY(DEF_ME(errorable))

    void me::dbgLog() const {
        WHEN(!buildFeature::config::isDbg()) .ret();
        log();
    }

    const std::string& me::getLevelName() const { return errLv::getName(getLv()); }
}
