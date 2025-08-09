#include "clog/when.hpp"

namespace by {
    BY(DEF_ME(__WHEN_OBJECT__))

    const me& me::get() {
        static me inner;
        return inner;
    }
}
