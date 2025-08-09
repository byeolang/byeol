#include "indep/helper/end.hpp"

namespace by {
    BY(DEF_ME(end))

    me::end(std::function<void(void)> l): _lambda(std::move(l)) {}

    me::~end() { _lambda(); }
}
