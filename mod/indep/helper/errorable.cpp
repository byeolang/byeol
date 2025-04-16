#include "errorable.hpp"
#include "buildFeature.hpp"

namespace nm {
    NM(DEF_ME(errorable))

    void me::dbgLog() const {
        WHEN(!buildFeature::config::isDbg()).ret();
        log();
    }

    const std::string& me::getLevelName() const { return errLv::getName(getLv()); }
}
