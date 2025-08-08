#include "core/worker/tokenDispatcher.hpp"

namespace by {

    BY_DEF_ME(tokenDispatcher)

    void me::add(nint newToken) { _toks.push_back(newToken); }

    void me::addFront(nint newToken) { _toks.insert(_toks.begin(), newToken); }

    nbool me::pop(nint& output) {
        WHEN(_toks.size() <= 0).ret(false);

        output = _toks.front();
        _toks.pop_front();
        return true;
    }

    ncnt me::len() const { return _toks.size(); }

    void me::rel() { _toks.clear(); }
}
