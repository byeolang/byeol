#include "errPassFilter.hpp"

namespace nm {
    NM(DEF_ME(errPassFilter))

    nbool me::filt(logLv::level lv, const std::string& tag) const { return lv <= logLv::ERR; }
}
