#include "dumSrcFile.hpp"

namespace nm {
    NM(DEF_ME(dumSrcFile))

    me::dumSrcFile(): super("", "") {}

    me& me::singleton() {
        static me inner;
        return inner;
    }
}
