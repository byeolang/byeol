#include "frontend/flag/flag.hpp"

#include "frontend/cli.hpp"

namespace by {

    BY_DEF_ME(flag)

    namespace {
        nbool _isOptionClustered(const std::string& arg) { return arg.size() > 2 && arg[0] == '-' && arg[1] != '-'; }
    }

    ncnt me::_parseOption(flagArgs& a, flagArgs& tray, deleteIndices& deletes) const {
        const std::string& arg = a[0];
        ncnt deleteOptionCnt = 0;
        for(const std::string& match: _getRegExpr()) {
            std::regex re(match);
            if(!std::regex_search(arg, re)) continue;

            std::string matchedArg = arg;
            deleteOptionCnt = 1;

            // check option clustring:
            if(_isOptionClustered(arg)) {
                WHEN(getArgCount() > 0) .exErr(OPTION_CANT_CLUSTERED).ret(0);

                // previous pushed argument will be removed after execution.
                // so, add additional argument with rest of string using `-[\w]` at the begin.
                // I confimred that length is more than 2 in `isOptionClustered()`
                matchedArg = std::string("-") + arg[1];
                a.push_back("-" + arg.substr(2));
            }

            tray.push_back(matchedArg);

            // handle option arguments:
            deleteOptionCnt += getArgCount();
            for(int n = 1; n < getArgCount(); n++)
                tray.push_back(a[n]);
            return deleteOptionCnt;
        }
        return 0;
    }

    me::res me::take(interpreter& ip, starter& s, cli& c, flagArgs& a) const {
        deleteIndices del;
        flagArgs tray;

        ncnt deleteOptionCnt = _parseOption(a, tray, del);
        WHEN(deleteOptionCnt <= 0).ret(NOT_MATCH);
        WHEN(tray.empty()) .ret(NOT_MATCH);

        res res = _onTake(tray, c, ip, s);
        _delArgs(a, deleteOptionCnt);
        return res;
    }

    ncnt me::getArgCount() const { return 0; }

    void me::_delArgs(flagArgs& a, ncnt deleteOptionCnt) const {
        // remove del in reverse order.
        for(int n = deleteOptionCnt - 1; n >= 0 ;n--)
            a.erase(a.begin() + n);
    }
} // namespace by
