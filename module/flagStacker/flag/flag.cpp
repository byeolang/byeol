#include "flagStacker/flag/flag.hpp"
#include "flagStacker/stacker.hpp"
#include <regex>
#include <iostream>

namespace by {
    typedef flag me;

    namespace {
        nbool _isOptionClustered(const std::string& arg) { return arg.size() > 2 && arg[0] == '-' && arg[1] != '-'; }
    }

    ncnt me::_parseOption(flagArgs& a, flagArgs& tray, stacker& s) const {
        const std::string& arg = a[0];
        ncnt deleteOptionCnt = 0;
        if(!canTake(arg)) return 0;

        std::string matchedArg = arg;
        deleteOptionCnt = 1;

        // check option clustring:
        if(_isOptionClustered(arg)) {
            if(getArgCount() > 0) return 0;

            // previous pushed argument will be removed after execution.
            // so, add additional argument with rest of string using `-[\w]` at the begin.
            // I confimred that length is more than 2 in `isOptionClustered()`
            matchedArg = std::string("-") + arg[1];
            a.push_back(std::string("-" + arg.substr(2)));
        }

        tray.push_back(matchedArg);

        // handle option arguments:
        if(a.size() < getArgCount() + 1) {
            std::cerr << std::string("Option `") + getName() + "`needs trailing arguments";
            return 0;
        }
        deleteOptionCnt += getArgCount();
        for(int n = 1; n < 1 + getArgCount(); n++)
            tray.push_back(a[n]);
        return deleteOptionCnt;
    }

    me::res me::take(flagArgs& a, stacker& s) {
        flagArgs tray;

        ncnt deleteOptionCnt = _parseOption(a, tray, s);
        if(deleteOptionCnt <= 0) return NOT_MATCH;
        if(tray.size() <= 0) return NOT_MATCH;

        res res = _onTake(tray);
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
            a.erase(a.begin() + n);
    }
} // namespace by
