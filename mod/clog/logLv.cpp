#include "logLv.hpp"

namespace nm {
    NM(DEF_ME(logLv))

    const std::string& me::getName(level lv) {
        switch(lv) {
            case ERR: {
                static std::string msg("E");
                return msg;
            }
            case WARN: {
                static std::string msg("W");
                return msg;
            }
            case INFO: {
                static std::string msg("I");
                return msg;
            }
        }

        static std::string msg("?");
        return msg;
    }
}
