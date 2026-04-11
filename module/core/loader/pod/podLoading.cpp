#include "core/loader/pod/podLoading.hpp"
#include "core/ast/pod.hpp"
#include "core/frame/thread.hpp"

namespace by {

    BY_DEF_ME(podLoading)

    void me::addPath(const std::string& filePath) { _paths.push_back(filePath); }

    void me::addPath(const std::vector<std::string> paths) {
        WHEN(&_paths == &paths) .ret();

        _paths.insert(_paths.end(), paths.begin(), paths.end());
    }

    void me::rel() { _paths.clear(); }

    void me::_tryDump(const errReport& rpt, const pod& p, ncnt prevCnt) const {
        WHEN(rpt.len() <= prevCnt).ret();

        // turn on temporary logger's disabled flag:
        //  podLoading occurs during verification or exapansaion in lazy.
        //  in that case, logger was set to be disabled.
        enablesZone zone(true); {
            BY_W_SCOPE("Loading `%s` pod at `%s` is failed", p.getManifest().name, util::joinVectorString(_getPaths()));
            rpt.log(prevCnt);
        }
    }

    const std::vector<std::string>& me::_getPaths() const { return _paths; }
}
