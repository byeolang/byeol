#include "bufferSrcFlag.hpp"

#include <iostream>

#include "../../cli.hpp"

namespace nm {

    NM_DEF_ME(bufferSrcFlag)

    const nchar* me::getName() const { return "-s"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    programming script can passed in as a string with this flag.)DESC";
    }

    const strings& me::_getRegExpr() const {
        static strings inner{"^\\-s$"};
        return inner;
    }

    ncnt me::_onContinuousArgCount() const { return 1; }

    me::res me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        if(tray.size() < 2) return NM_E("invalid flagArgument size < 2"), EXIT_PROGRAM;

        ip.addSupply(*new bufSupply(tray[1]));
        return MATCH;
    }
}
