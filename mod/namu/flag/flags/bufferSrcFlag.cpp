#include "bufferSrcFlag.hpp"
#include "../../cli.hpp"
#include <iostream>

namespace nm {

    NAMU_DEF_ME(bufferSrcFlag)

    const nchar* me::getName() const {
        return "-s";
    }

    const nchar* me::getDescription() const {
        return R"DESC(
    programming script can passed in as a string with this flag.)DESC";
    }

    const nchar* me::_getRegExpr() const {
        return "^\\-s$";
    }

    ncnt me::_onContinuousArgCount() const {
        return 1;
    }

    nbool me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        if(tray.size() < 2) return NAMU_E("invalid flagArgument size < 2"), false;

        ip.addSupply(*new bufSupply(tray[1]));
        return true;
    }
}
