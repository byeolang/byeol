#pragma once

#include <list>

#include "leaf/common.hpp"

namespace by {
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
