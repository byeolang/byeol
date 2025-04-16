#include "errLv.hpp"

namespace nm {
    NM(DEF_ME(errLv))

    const std::string& me::getName(level lv) {
        switch(lv) {
            case ERR: {
                static std::string msg("ERR");
                return msg;
            }
            case WARN: {
                static std::string msg("WARN");
                return msg;
            }
            case INFO: {
                static std::string msg("INFO");
                return msg;
            }
        }

        static std::string msg("?");
        return msg;
    }
}
