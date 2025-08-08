#include "clog/filter/errPassFilter.hpp"

namespace by {
    NM(DEF_ME(errPassFilter))

    nbool me::filt(errLv::level lv, const std::string& tag) const { return lv <= errLv::ERR; }
}
