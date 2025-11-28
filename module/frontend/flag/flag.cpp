#include "frontend/flag/flag.hpp"

#include "frontend/cli.hpp"

namespace by {

    BY_DEF_ME(flag)

    me::res me::take(interpreter& ip, starter& s, cli& c, flagArgs& a) const {
        std::vector<int> del;
        flagArgs tray;

        for(int n = 0; n < a.size(); n++) {
            for(const std::string& match: _getRegExpr()) {
                std::regex re(match);
                if(!std::regex_match(a[n], re)) continue;

                tray.push_back(a[n]);
                del.push_back(n);

                for(int cn = ++n; cn < n + getArgCount(); cn++) {
                    tray.push_back(a[cn]);
                    del.push_back(cn);
                }
                break;
            }
        }
        if(tray.empty()) return NOT_MATCH;

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
