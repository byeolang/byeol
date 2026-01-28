/// @file
#pragma once

#include <list>

#include "stela/common.hpp"

namespace by {
    /** @ingroup stela
     *  @brief Token dispatcher for managing token queue during parsing
     *  @details Manages a queue of tokens for lookahead and token buffering
     *  during the parsing process. Supports adding tokens to front and back.
     */
    class _nout stelaTokenDispatcher {
    public:
        void add(nint newToken);
        void addFront(nint newToken);
        /**
         * @brief Removes a token from the front of the queue.
         * @param output Reference to store the popped token ID.
         * @return true if a token was successfully popped, false if the queue was empty.
         */
        nbool pop(nint& output);

        ncnt len() const;

        void rel();

    private:
        std::list<nint> _toks;
    };
}
