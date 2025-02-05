#include "convergence.hpp"

namespace nm {
    NM(DEF_ME(convergence))

    me::convergence(baseObj& obj, baseFunc& fun, onConverge closure):
        super(obj, fun, closure) {}

    nbool me::convergeWithoutFrame() { return _getClosure()(); }
}
