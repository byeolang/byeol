#include "end.hpp"

namespace nm {
    NM(DEF_ME(end))

    me::end(std::function<void(void)> l): _lambda(l) {}

    me::~end() { _lambda(); }
}
