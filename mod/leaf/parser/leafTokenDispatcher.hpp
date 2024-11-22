#pragma once

#include <list>

#include "../common.hpp"

namespace nm {
    class _nout leafTokenDispatcher {
    public:
        void add(nint newToken);
        void addFront(nint newToken);
        nbool pop(nint& output);

        ncnt len() const;

        void rel();

    private:
        std::list<nint> _toks;
    };
}
