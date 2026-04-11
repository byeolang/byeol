#include "core/loader/pod/trustPodLoading.hpp"

namespace by {

    BY_DEF_ME(trustPodLoading)

    nbool me::expand(errReport& rpt, pod&) { return true; }

    nbool me::verify(errReport& rpt, pod&) { return true; }
}
