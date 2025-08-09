#include "indep/macro/when.hpp"

namespace by {
    BY(DEF_ME(__WHEN_OBJECT__))

    const me& me::get() {
        static me inner;
        return inner;
    }

    void me::ret() const {}

    void me::crash() const { abort(); }
}
