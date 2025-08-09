#include "core/ast/src/dumSrcFile.hpp"

namespace by {
    BY(DEF_ME(dumSrcFile))

    me::dumSrcFile(): super("", "") {}

    me& me::singleton() {
        static me inner;
        return inner;
    }
}
