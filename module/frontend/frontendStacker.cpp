#include "frontend/frontendStacker.hpp"
#include "frontend/flags.hpp"

namespace by {
    BY(DEF_ME(frontendStacker))

    me::frontendStacker(cli& c): _cli(c) {}

    void me::_initFlags(flags& tray) const {
        tray.push_back(new bufferSrcFlag(_cli));
        tray.push_back(new helpFlag(_cli));
        tray.push_back(new verFlag());
        tray.push_back(new verboseFlag(_cli));
        tray.push_back(new logStructureFlag(_cli));
        tray.push_back(new fileFlag(_cli));
        tray.push_back(new dirFlag(_cli));
    }
}