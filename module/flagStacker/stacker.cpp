#include "flagStacker/stacker.hpp"
#include <cstdarg>

namespace by {
    typedef stacker me;

    me::~stacker() {
        for(auto f: _flags)
            delete f;
    }

    flag::res me::take(flagArgs& a) { return take(a, *this); }

    flag::res me::take(flagArgs& a, stacker&) {
        while(a.size() > 0) {
            flag::res r = flag::NOT_MATCH;
            for(const auto& op: getFlags()) {
                r = op->take(a, *this);
                if(r == flag::EXIT_PROGRAM) return r;
                if(r == flag::MATCH) break;
            }
            if(r == flag::NOT_MATCH) return r; // if all flags couldn't match the first argument.
        }

        return flag::MATCH;
    }

    nbool me::canTake(const std::string& pattern) const {
        for(const auto* f : _flags)
            if(f->canTake(pattern)) return true;
        return false;
    }

    const flags& me::getFlags() const {
        if(_flags.size() > 0) return _flags;
        _initFlags(_flags);
        return _flags;
    }
} // namespace by
