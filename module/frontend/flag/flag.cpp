#include "frontend/flag/flag.hpp"

#include "frontend/cli.hpp"

namespace by {

    BY_DEF_ME(flag)

    namespace {
        nbool _isOptionClustered(const std::string& arg) { return arg.size() > 2 && arg[0] == '-' && arg[1] != '-'; }
    }

    ncnt me::_parseOption(flagArgs& a, flagArgs& tray, errReport& rpt) const {
        const std::string& arg = a[0].get();
        ncnt deleteOptionCnt = 0;
        WHEN(!canTake(arg)).ret(0);

        std::string matchedArg = arg;
        deleteOptionCnt = 1;

        // check option clustring:
        if(_isOptionClustered(arg)) {
            WHEN(getArgCount() > 0) .exErr(OPTION_CANT_CLUSTERED, rpt).ret(0);

            // previous pushed argument will be removed after execution.
            // so, add additional argument with rest of string using `-[\w]` at the begin.
            // I confimred that length is more than 2 in `isOptionClustered()`
            matchedArg = std::string("-") + arg[1];
            a.add(new nStr("-" + arg.substr(2)));
        }

        tray.add(new nStr(matchedArg));

        // handle option arguments:
        WHEN(a.len() < getArgCount() + 1) .exErr(OPTION_NEEDS_TRAILING_ARG, rpt, getName()).ret(0);
        deleteOptionCnt += getArgCount();
        for(int n = 1; n < 1 + getArgCount(); n++)
            tray.add(a[n]);
        return deleteOptionCnt;
    }

    me::res me::take(interpreter& ip, starter& s, cli& c, flagArgs& a, errReport& rpt) const {
        flagArgs tray;

        ncnt deleteOptionCnt = _parseOption(a, tray, rpt);
        WHEN(deleteOptionCnt <= 0) .ret(NOT_MATCH);
        WHEN(tray.isEmpty()) .ret(NOT_MATCH);

        res res = _onTake(tray, c, ip, s, rpt);
        _delArgs(a, deleteOptionCnt);
        return res;
    }

    nbool me::canTake(const std::string& pattern) const {
        for(const std::string& match: _getRegExpr()) {
            std::regex re(match);
            if(std::regex_search(pattern, re)) return true;
        }

        return false;
    }

    ncnt me::getArgCount() const { return 0; }

    void me::_delArgs(flagArgs& a, ncnt deleteOptionCnt) const {
        // remove del in reverse order.
        for(int n = deleteOptionCnt - 1; n >= 0; n--)
            a.del(n);
    }
} // namespace by
