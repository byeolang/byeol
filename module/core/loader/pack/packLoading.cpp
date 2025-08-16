#include "core/loader/pack/packLoading.hpp"

#include "core/frame/thread.hpp"

namespace by {

    BY_DEF_ME(packLoading)

    void me::addPath(const std::string& filePath) { _paths.push_back(filePath); }

    void me::addPath(const std::vector<std::string> paths) {
        WHEN(&_paths == &paths) .ret();

        _paths.insert(_paths.end(), paths.begin(), paths.end());
    }

    void me::rel() { _paths.clear(); }

    const std::vector<std::string>& me::_getPaths() const { return _paths; }
}
