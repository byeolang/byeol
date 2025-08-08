#include "clog/when.hpp"

namespace by {
    NM(DEF_ME(__WHEN_OBJECT__))

    const me& me::get() {
        static me inner;
        return inner;
    }
}
