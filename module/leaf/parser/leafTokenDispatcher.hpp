/// @file
#pragma once

#include <list>

#include "leaf/common.hpp"

namespace by {
    /// @ingroup leaf
    /// @brief Token dispatcher for managing token queue during parsing
    /// @details Manages a queue of tokens for lookahead and token buffering
    /// during the parsing process. Supports adding tokens to front and back.
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
