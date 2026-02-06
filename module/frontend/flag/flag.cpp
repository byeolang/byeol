#include "frontend/flag/flag.hpp"

#include "frontend/cli.hpp"

namespace by {

    BY_DEF_ME(flag)

    namespace {
        nbool _isOptionClustered(const std::string& arg) { return arg.size() > 2 && arg[0] == '-' && arg[1] != '-'; }
    }

    nbool me::_parseOption(flagArgs& a, flagArgs& tray, deleteIndices& deletes) const {
        for(int n = 0; n < a.size(); n++) {
            for(const std::string& match: _getRegExpr()) {
                std::regex re(match);
                std::string& arg = a[n];
                if(!std::regex_search(arg, re)) continue;

                // check option clustring:
                if(_isOptionClustered(arg)) {
                    WHEN(getArgCount() > 0) .exErr(OPTION_CANT_CLUSTERED).ret(false);

                    tray.push_back(std::string("-") + arg[1]);
                    // remove `-[\w]` at the begin.
                    // I confimred that length is more than 2 in `isOptionClustered()`
                    arg = "-" + arg.substr(2);
                    return true;
                }

                tray.push_back(arg);
                deletes.push_back(n);

                // handle option arguments:
                for(int cn = ++n; cn < n + getArgCount(); cn++) {
                    tray.push_back(a[cn]);
                    deletes.push_back(cn);
                }
                return true;
            }
        }
        return false;
    }

    me::res me::take(interpreter& ip, starter& s, cli& c, flagArgs& a) const {
        deleteIndices del;
        flagArgs tray;

        if(!_parseOption(a, tray, del)) return NOT_MATCH;
        WHEN(tray.empty()) .ret(NOT_MATCH);

        res res = _onTake(tray, c, ip, s);
        if(res == MATCH) _delArgs(a, del);
        return res;
    }

    ncnt me::getArgCount() const { return 0; }

    void me::_delArgs(flagArgs& a, std::vector<int> indices) const {
        // remove del in reverse order.
        for(auto e = indices.rbegin(); e != indices.rend(); ++e)
            a.erase(a.begin() + *e);
    }
} // namespace by
