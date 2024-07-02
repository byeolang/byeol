#include "flag.hpp"
#include "../cli.hpp"

namespace nm {

    NAMU_DEF_ME(flag)

    nbool me::take(interpreter& ip, starter& s, cli& c, flagArgs& a) const {
        std::regex re(_getRegExpr());
        std::vector<int> del;
        flagArgs tray;

        for(int n=0; n < a.size() ;n++) {
            if(!std::regex_match(a[n], re)) continue;

            tray.push_back(a[n]);
            del.push_back(n);

            for(int cn=++n; cn < n + _onContinuousArgCount() ;cn++) {
                tray.push_back(a[cn]);
                del.push_back(cn);
            }
        }
        if(tray.empty()) return false;

        nbool res = _onTake(tray, c, ip, s);
        if(!res) return false;

        _delArgs(a, del);
        return res;
    }

    ncnt me::_onContinuousArgCount() const {
        return 0;
    }

    void me::_delArgs(flagArgs& a, std::vector<int> indices) const {
        // remove del in reverse order.
        for(auto e=indices.rbegin(); e != indices.rend() ;++e)
            a.erase(a.begin() + *e);
    }
}
